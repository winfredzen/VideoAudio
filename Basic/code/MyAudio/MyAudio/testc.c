//
//  testc.c
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#include "testc.h"

void haha() {
    printf("This is c function\n");
    
    av_log_set_level(AV_LOG_DEBUG);
    av_log(NULL, AV_LOG_DEBUG, "Hello FFmpeg!!!");
    
    return;
}
