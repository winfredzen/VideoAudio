//
//  testc.h
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

#ifndef testc_h
#define testc_h

#include <stdio.h>
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include <unistd.h>
#include "libswresample/swresample.h" //重采样

//录制视频
void record_video(void);

//设置录制状态，用于停止录制
void set_status(int status);

#endif /* testc_h */
