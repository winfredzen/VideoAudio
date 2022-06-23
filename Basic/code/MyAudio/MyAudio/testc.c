//
//  testc.c
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#include "testc.h"

//录制状态
static int rec_status = 0;

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

    //来时录制
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
        printf("pkt size %d(%p) %d\n", pkt.size, pkt.data, count);

        //写入文件
        fwrite(pkt.data, pkt.size, 1, file);
        fflush(file);

        av_packet_unref(&pkt);
    }
    
    //关闭问价
    fclose(file);
    
    //释放上下文
    avformat_close_input(&fmt_ctx);
    
    
    av_log(NULL, AV_LOG_DEBUG, "Finish!!!\n");
    return;
}

void set_status(int status) {
    rec_status = status;
}
