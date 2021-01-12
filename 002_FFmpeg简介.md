# FFmpeg简介

FFmpeg官网地址：

+ [FFmpeg](https://ffmpeg.org/)

FFmpeg既是一款音视频编解码工具，同时也是一组音视频编解码开发套件，作为编解码开发套件，它为开发者提供了丰富的音视频处理的调用接口。

FFmpeg提供了多种媒体格式的封装和解封装，包括多种音视频编码、多种协议的流媒体、多种色彩格式转换、多种采样率转换、多种码率转换等；FFmpeg框架提供了多种丰富的插件模块，包含封装与解封装的插件、编码与解码的插件等。

FFmpeg中的`FF`指的是`Fast Forward`



**`FFmpeg`的基本组成**

![009](https://github.com/winfredzen/VideoAudio/blob/main/images/009.png)

+ `AVFormat` - `AVFormat`中实现了目前多媒体领域中的绝大多数媒体封装格式，包括封装和解封装，如MP4、FLV、KV、TS等文件封装格式，RTMP、RTSP、MMS、HLS等网络协议封装格式。FFmpeg是否支持某种媒体封装格式，取决于编译时是否包含了该格式的封装库。根据实际需求，可进行媒体封装格式的扩展，增加自己定制的封装格式，即在`AVFormat`中增加自己的封装处理模块。
+ 



























