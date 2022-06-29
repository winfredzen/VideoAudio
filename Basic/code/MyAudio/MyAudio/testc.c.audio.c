//
//  testc.c
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#include "testc.h"

//录制状态
static int rec_status = 0;

//编码
static void encode(AVCodecContext *codec_ctx, AVFrame *frame, AVPacket *pkt, FILE *output) {
    //将数据送入编码器
    int ret = 0;
    ret = avcodec_send_frame(codec_ctx, frame);
    while (ret >= 0) {//数据设置成功
        //获取编码后的数据，如果成功，需要重复获取，直到失败为止
        ret = avcodec_receive_packet(codec_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            printf("Error, encoding audio frame!!!\n");
            exit(-1);
        }

        //写入文件
        //fwrite(pkt.data, pkt.size, 1, file);//pcm
        //fwrite(dst_data[0], 1, dst_linesize, file);//重采样
        fwrite(pkt->data, 1, pkt->size, output);//AAC
        fflush(output);
    }
}

static AVCodecContext* open_codec() {
    //avcodec_find_encoder(AV_CODEC_ID_AAC);
    AVCodec *codec = avcodec_find_encoder_by_name("libfdk_aac");
    //创建上下文
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    codec_ctx->sample_fmt = AV_SAMPLE_FMT_S16;//位深 （AAC的采样大小就是16位）
    codec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;//channel
    codec_ctx->channels = 2;
    codec_ctx->sample_rate = 44100;//采样率
    codec_ctx->bit_rate = 0;//AAC_LC:128K AAC_HE:64k AAC_HE_V2: 32K
    codec_ctx->profile = FF_PROFILE_AAC_HE;
    //打开编码器
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        printf("avcodec_open2(codec_ctx, codec, NULL) < 0");
    }
    return codec_ctx;
}

static SwrContext* init_swr() {
    //重采样 - 这里输入使用mac外接耳机的麦克风
    SwrContext *swr_ctx = NULL;
    swr_ctx = swr_alloc_set_opts(NULL,                  //ctx
                                 AV_CH_LAYOUT_STEREO,   //输出channel布局
                                 AV_SAMPLE_FMT_S16,     //输出的采样格式
                                 44100,                 //输出的采样率
                                 AV_CH_LAYOUT_MONO,     //输入的....
                                 AV_SAMPLE_FMT_FLT,
                                 44100,
                                 0,                     //日志相关...
                                 NULL);
    if (swr_ctx == NULL) {
        printf("swr_ctx == NUL");
    }
    //初始化
    if (swr_init(swr_ctx) < 0) {
        printf("swr_init < 0");
    }
    return swr_ctx;
}

//打开设备
static AVFormatContext* open_dev() {
    int ret = 0;
    //出错信息
    char errors[1024] = {0, };
    
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    
    //格式[[VIDEO]:[AUDIO]] 下面的格式表示音频设备
    char *devciename = ":0";
    //get格式
    AVInputFormat *inputFormat = av_find_input_format("avfoundation");
    
    //打开输入设备
    ret = avformat_open_input(&fmt_ctx, devciename, inputFormat, &options);
    
    if (ret < 0) {
        av_strerror(ret, errors, 1024);
        fprintf(stderr, "Failded to open device [%d]%s\n", ret, errors);
        return NULL;
    }
    return fmt_ctx;
}

//创建frame
static AVFrame* create_frame() {
    AVFrame *frame = NULL;
    //音频输入数据
    frame = av_frame_alloc();
    if (!frame) {
        printf("frame == NUL");
        goto __ERROR;
    }
    //设置参数
    frame->nb_samples = 512;//单通道的一个音频帧采样数
    frame->format = AV_SAMPLE_FMT_S16; //每个采样大小
    frame->channel_layout = AV_CH_LAYOUT_STEREO;//通道
    av_frame_get_buffer(frame, 0);
    
    if (!frame->data[0]) {
        printf("frame->buf[0] == NULL");
        goto __ERROR;
    }
    return frame;
    
    //处理出错，释放内存
__ERROR:
    if (frame) {
        av_frame_free(&frame);
    }
    return NULL;
}

static void alloc_data_4_resample(uint8_t ***src_data,
                           int *src_linesize,
                           uint8_t ***dst_data,
                           int *dst_linesize) {
    //2048/4/1 = 512 采集的大小（pkt.size）/位深（浮点数4个字节）/通道数
    av_samples_alloc_array_and_samples(src_data, //输出缓冲区地址
                                       src_linesize,//缓冲区的大小
                                       1,//通道个数
                                       512,//采样个数
                                       AV_SAMPLE_FMT_FLT,//采样格式
                                       0);
    
    //2048/2/2 = 512 采集的大小（pkt.size）/位深（浮点数2个字节）/通道数
    av_samples_alloc_array_and_samples(dst_data, //输出缓冲区地址
                                       dst_linesize,//缓冲区的大小
                                       2,//通道个数
                                       512,//采样个数
                                       AV_SAMPLE_FMT_S16,//采样格式
                                       0);
}

static void free_data_4_resample(uint8_t **src_data, uint8_t **dst_data){
    //释放输入输出缓冲区
    if (src_data) {
        av_freep(&src_data[0]);
    }
    av_freep(&src_data);
    if (dst_data) {
        av_freep(&dst_data[0]);
    }
    av_freep(&dst_data);
}

static void read_data_and_encode(AVFormatContext *fmt_ctx, SwrContext *swr_ctx, AVCodecContext *codec_ctx, FILE *outFile) {
    int ret = 0;
    
    //创建输入缓冲区
    uint8_t **src_data = NULL;
    int src_linesize = 0;
    
    //创建输出缓冲区
    uint8_t **dst_data = NULL;
    int dst_linesize = 0;
    
    //读数据
    AVPacket pkt;
    AVFrame *frame = NULL;
    AVPacket *newpkt = NULL;

    
    av_init_packet(&pkt);
    
    //音频输入数据
    frame = create_frame();
    if (!frame) {
        printf("frame == NUL");
        goto __ERROR;
    }
    
    //音频输出数据
    newpkt = av_packet_alloc();//分配编码后的数据空间
    if (!newpkt) {
        printf("newpkt == NUL");
        goto __ERROR;
    }
    
    //分配重采样输入输出缓冲区
    alloc_data_4_resample(&src_data, &src_linesize, &dst_data, &dst_linesize);
    
    //实时录制
    while (rec_status) {
        ret = av_read_frame(fmt_ctx, &pkt);
        if(ret == -35) {
            ret = 0;
            av_packet_unref(&pkt);
            continue;
        }

        if(ret < 0){
            av_packet_unref(&pkt);
            break;
        }
        printf("pkt size %d(%p)\n", pkt.size, pkt.data); //pkt size 2048(0x1048d3a00) 0
        
        //内存拷贝，将pkt数据复制给源数据
        memcpy((void *)src_data[0], pkt.data, pkt.size);

        //重采样
        swr_convert(swr_ctx,//重采样的上下文
                    dst_data,//输出的结果
                    512,//每个通道的采样数
                    (const uint8_t **)src_data,//输入缓冲区
                    512);
        
        //重采样后的数据放入AVFrame中
        memcpy((void *)frame->data[0], dst_data[0], dst_linesize);
        
        //编码
        encode(codec_ctx, frame, newpkt, outFile);
        
        av_packet_unref(&pkt);
    }
    
    //编码，强制将编码器中缓冲的数据进行编码输出
    encode(codec_ctx, NULL, newpkt, outFile);

__ERROR:
    if (frame) {
        av_frame_free(&frame);
    }
    
    if (newpkt) {
        av_packet_free(&newpkt);
    }
   
    //释放重采样缓冲区
    free_data_4_resample(src_data, dst_data);
}

void record_audio() {
    printf("This is c function\n");
    
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    SwrContext *swr_ctx = NULL;


    av_log_set_level(AV_LOG_DEBUG);
    av_log(NULL, AV_LOG_DEBUG, "Hello FFmpeg!!!\n");
    
    //注册设备
    avdevice_register_all();
    
    rec_status = 1;
    
    
    //创建文件
    //char *out = "/Users/wangzhen/Downloads/audio.pcm";
    char *out = "/Users/wangzhen/Downloads/audio.aac";
    FILE *file = fopen(out, "wb+");
    if (!file) {
        printf("file == NUL");
        goto __ERROR;
    }
    
    //打开设备
    fmt_ctx = open_dev();
    if (!fmt_ctx) {
        printf("fmt_ctx == NUL");
        goto __ERROR;
    }
    
    //打开编码器上下文
    codec_ctx = open_codec();
    if (!codec_ctx) {
        printf("codec_ctx == NUL");
        goto __ERROR;
    }
    
    //重采样初始化 - 这里输入使用mac外接耳机的麦克风
    swr_ctx = init_swr();
    if (!swr_ctx) {
        printf("Error, Failded to alloc swr_ctx");
        goto __ERROR;
    }
    
    //读取数据并编码
    read_data_and_encode(fmt_ctx, swr_ctx, codec_ctx, file);
    
__ERROR:
    //释放重采样上下文
    if (swr_ctx) {
        swr_free(&swr_ctx);
    }
    
    if (codec_ctx) {
        avcodec_free_context(&codec_ctx);
    }

    //关闭设备释放上下文
    if (fmt_ctx) {
        avformat_close_input(&fmt_ctx);
    }
    
    //关闭文件
    if (file) {
        fclose(file);
    }
    

    av_log(NULL, AV_LOG_DEBUG, "Finish!!!\n");
    return;
}



void set_status(int status) {
    rec_status = status;
}