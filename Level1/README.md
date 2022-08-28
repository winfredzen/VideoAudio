# FFmpeg

主要记录学习[经典再升级-FFmpeg音视频核心技术全面精讲+实战](https://coding.imooc.com/class/279.html)的过程



**FFmpeg处理音视频流程**

![001](https://github.com/winfredzen/VideoAudio/blob/main/Level1/images/001.png)

1.输入文件 - 具有封装格式的文件，mp4等，类似一个盒子

2.demuxer - 打开这个盒子，暴露出来的数据称为编码数据，它是压缩后的数据

3.decoder - 压缩数据解码，变成为原始数据

4.encoder - 原始数据再编码

5.muxer - 编码数据再封装



