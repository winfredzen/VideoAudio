//
//  testc.c
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#include "testc.h"
#include <librtmp/rtmp.h>


//录制状态
static int rec_status = 0;

static FILE * open_flv(char *flvaddr) {
    FILE *fp = NULL;
    
    return fp;
}

static RTMP* connect_rtmp_server(char *rtmpaddr) {
    RTMP *rtmp = NULL;
    return rtmp;
}

static void send_data(FILE *fp, RTMP *rtmp) {
    
}

void push_stream() {
    char *flv = NULL;
    char *rtmpaddr = NULL;
    
    //1.读flv文件
    FILE *fp = open_flv(flv);
    
    //2.连接rtmp服务器
    RTMP *rtmp = connect_rtmp_server(rtmpaddr);
    
    //3.推流
    send_data(fp, rtmp);
    
    printf("push_stream\n");
}




void set_status(int status) {
    rec_status = status;
}
