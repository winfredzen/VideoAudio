//
//  testc.c
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#include "testc.h"

//录制状态
static int rec_status = 0;

//打开设备
static AVFormatContext* open_dev() {
    int ret = 0;
    //出错信息
    char errors[1024] = {0, };
    
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    
    //格式[[VIDEO]:[AUDIO]]，只采集视频 0 -表示摄像头 1- 表示桌面
    char *devciename = "0";
    
    //register audio device
    avdevice_register_all();
    
    //get格式
    AVInputFormat *inputFormat = av_find_input_format("avfoundation");
    
    //采集视频需要设置一些参数
    enum AVPixelFormat av_fmt = AV_PIX_FMT_NV12;
    av_dict_set(&options, "video_size", "640x480", 0);//分辨率
    av_dict_set(&options, "framerate", "30", 0);//帧率
    av_dict_set(&options,"pixel_format","nv12", 0);//采样格式 nv12=yuv420sp
    
    
    //打开输入设备
    ret = avformat_open_input(&fmt_ctx, devciename, inputFormat, &options);
    if (ret < 0) {
        av_strerror(ret, errors, 1024);
        fprintf(stderr, "Failded to open device [%d]%s\n", ret, errors);
        return NULL;
    }
    return fmt_ctx;
}


void record_video() {
    printf("This is c function\n");
    
    AVFormatContext *fmt_ctx = NULL;
    int ret = 0;
    //读数据
    AVPacket pkt;

    av_log_set_level(AV_LOG_DEBUG);
    av_log(NULL, AV_LOG_DEBUG, "Hello FFmpeg!!!\n");
    
    rec_status = 1;
    
    //创建文件
    char *out = "/Users/wangzhen/Downloads/video.yuv";
    FILE *file = fopen(out, "wb+");
    if (!file) {
        printf("file == NUL");
        goto __ERROR;
    }
    
    //打开设备
    fmt_ctx = open_dev();
    if (!fmt_ctx) {
        printf("fmt_ctx == NULL\n");
        goto __ERROR;
    }
    
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

        //写文件
        //fwrite(pkt.data, 1, pkt.size, file);
        fwrite(pkt.data, 1, 460800, file);
        fflush(file);
        
        av_packet_unref(&pkt);
    }
    
__ERROR:

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
