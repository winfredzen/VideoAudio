# FFmpeg简介

FFmpeg官网地址：

+ [FFmpeg](https://ffmpeg.org/)



其它资源：

+ [FFMPEG](https://blog.csdn.net/leixiaohua1020/category_1360795.html)



FFmpeg既是一款音视频编解码工具，同时也是一组音视频编解码开发套件，作为编解码开发套件，它为开发者提供了丰富的音视频处理的调用接口。

FFmpeg提供了多种媒体格式的封装和解封装，包括多种音视频编码、多种协议的流媒体、多种色彩格式转换、多种采样率转换、多种码率转换等；FFmpeg框架提供了多种丰富的插件模块，包含封装与解封装的插件、编码与解码的插件等。

FFmpeg中的`FF`指的是`Fast Forward`



**`FFmpeg`的基本组成**

![009](https://github.com/winfredzen/VideoAudio/blob/main/images/009.png)

+ `AVFormat` - `AVFormat`中实现了目前多媒体领域中的绝大多数媒体封装格式，包括**封装**和**解封装**，如MP4、FLV、KV、TS等文件封装格式，RTMP、RTSP、MMS、HLS等网络协议封装格式。FFmpeg是否支持某种媒体封装格式，取决于编译时是否包含了该格式的封装库。根据实际需求，可进行媒体封装格式的扩展，增加自己定制的封装格式，即在`AVFormat`中增加自己的封装处理模块。
+ `AVCodec` - `AVCodec`中实现了目前多媒体领域绝大多数常用的编解码格式，既支持**编码**，也支持**解码**。`AVCodec`除了支持MPEG4、AAC、MJPEG等自带的媒体编解码格式之外，还支持第三方的编解码器，如`H.264（AVC）`编码，需要使用`x264`编码器；`H.265（HEVC）`编码，需要使用`x265`编码器；`MP3（mp3lame）`编码，需要使用`libmp3lame`编码器。如果希望增加自己的编码格式，或者硬件编解码，则需要在`AVCodec`中增加相应的编解码模块
+ `AVFilter` - `AVFilter`库提供了一个通用的音频、视频、字幕等滤镜处理框架
+ `swscale` - swscale模块提供了高级别的图像转换API，例如它允许进行图像缩放和像素格式转换，常见于将图像从`1080p`转换成`720p`或者`480p`等的缩放，或者将图像数据从`YUV420P`转换成`YUYV`，或者`YUV`转`RGB`等图像格式转换
+ `swresample` - `swresample`模块提供了高级别的音频重采样API。例如它允许操作音频采样、音频通道布局转换与布局调整
+ `AVDevice` - 输入输出设备库，比如，需要编译出播放声音或者视频的工具`ffplay`，就需要确保该模块是打开的，同时也需要`libSDL`的预先编译，因为该设备模块播放声音与播放视频使用的都是`libSDL`库
+ PostProc - 该模块可用于进行后期处理，当我们使用`AVFilter`的时候需要打开该模块的开关，因为Filter中会使用到该模块的一些基础函数



























