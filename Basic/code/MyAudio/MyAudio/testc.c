//
//  testc.c
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#include "testc.h"

//录制状态
static int rec_status = 0;

SwrContext* init_swr() {
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

void record_audio() {
    printf("This is c function\n");
    
    av_log_set_level(AV_LOG_DEBUG);
    av_log(NULL, AV_LOG_DEBUG, "Hello FFmpeg!!!\n");
    
    AVFormatContext *fmt_ctx = NULL;
    
    //格式[[VIDEO]:[AUDIO]] 下面的格式表示音频设备
    char *devciename = ":0";
    
    AVDictionary *options = NULL;
    
    //出错信息
    char errors[1024];
    
    //注册设备
    avdevice_register_all();
    
    //get格式
    AVInputFormat *inputFormat = av_find_input_format("avfoundation");
    
    //打开输入设备
    int ret = avformat_open_input(&fmt_ctx, devciename, inputFormat, &options);
    
    if (ret < 0) {
        av_strerror(ret, errors, 1024);
        printf(stderr, "Failded to open device [%d]%s\n", ret, errors);
        return;
    }
    
    //创建文件
    char *out = "/Users/wangzhen/Downloads/audio.pcm";
    FILE *file = fopen(out, "wb+");
    if (file == NULL) {
        printf("file == NUL");
    }
    
    //读数据
    AVPacket pkt;
    av_init_packet(&pkt);
    int count = 0;
    ret = 0;

    //重采样初始化 - 这里输入使用mac外接耳机的麦克风
    SwrContext *swr_ctx = init_swr();
    
    //创建输入缓冲区
    uint8_t **src_data = NULL;
    int src_linesize = 0;
    //2048/4/1 = 512 采集的大小（pkt.size）/位深（浮点数4个字节）/通道数
    av_samples_alloc_array_and_samples(&src_data, //输出缓冲区地址
                                       &src_linesize,//缓冲区的大小
                                       1,//通道个数
                                       512,//采样个数
                                       AV_SAMPLE_FMT_FLT,//采样格式
                                       0);
    
    //创建输出缓冲区
    uint8_t **dst_data = NULL;
    int dst_linesize = 0;
    //2048/2/2 = 512 采集的大小（pkt.size）/位深（浮点数4个字节）/通道数
    av_samples_alloc_array_and_samples(&dst_data, //输出缓冲区地址
                                       &dst_linesize,//缓冲区的大小
                                       2,//通道个数
                                       512,//采样个数
                                       AV_SAMPLE_FMT_S16,//采样格式
                                       0);
    
    //实时录制
    rec_status = 1;
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
        printf("pkt size %d(%p) %d\n", pkt.size, pkt.data, count); //pkt size 2048(0x1048d3a00) 0
        
        //内存拷贝，将pkt数据复制给源数据
        memcpy((void *)src_data[0], pkt.data, pkt.size);

        //重采样
        swr_convert(swr_ctx,//重采样的上下文
                    dst_data,//输出的结果
                    512,//每个通道的采样数
                    (const uint8_t **)src_data,//输入缓冲区
                    512);
        
        //写入文件
        //fwrite(pkt.data, pkt.size, 1, file);
        fwrite(dst_data[0], 1, dst_linesize, file);
        fflush(file);

        av_packet_unref(&pkt);
    }
    
    //关闭文件
    fclose(file);
    
    //释放输入输出缓冲区
    if (src_data) {
        av_freep(&src_data[0]);
    }
    av_freep(&src_data);
    if (dst_data) {
        av_freep(&dst_data[0]);
    }
    av_freep(&dst_data);
    //释放重采样上下文
    swr_free(&swr_ctx);
    
    //释放上下文
    avformat_close_input(&fmt_ctx);
    
    
    av_log(NULL, AV_LOG_DEBUG, "Finish!!!\n");
    return;
}

void set_status(int status) {
    rec_status = status;
}
