# 音视频基础

主要记录[编程必备基础-音视频小白系统入门课](https://coding.imooc.com/class/415.html)课程学习

开发中遇到的一些问题点，可参考作者的说明：

+ [音视频系统入门常见问题](https://avdancedu.com/631d466a/)



## FFmpeg

+ [https://ffmpeg.org/](https://ffmpeg.org/)
+ [https://trac.ffmpeg.org/](https://trac.ffmpeg.org/)

+ [http://www.ffmpeg.org/doxygen/trunk/examples.html](http://www.ffmpeg.org/doxygen/trunk/examples.html) - FFmpeg例子

## 资源

+ [秒懂音视频开发](https://www.cnblogs.com/mjios/category/1938094.html)
+ [test-videos](https://test-videos.co.uk/)

+ [audio_video_streaming](https://github.com/0voice/audio_video_streaming)

+ [零基础入门：实时音视频技术基础知识全面盘点](http://www.52im.net/thread-3079-1-1.html)



## Linux基础知识

`pwd` - 当前路径

`mkdir` - 创建目录, `mkdir -p 1/2/3`创建多层目录

`ls -alt` 列出当前所有的目录，`a`表示所有，`l`表示列表，`t`表示倒序

![001](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/001.png)

`cp` - 拷贝

`rm` - 删除，`rm -rf`循环删除，`-f`强制删除



安装工具

+ apt - Ubuntu下的安装工具
+ brew - Mac Os
+ yum - Centos



`brew search vim`搜索vim

`brew install vim`安装vim



### vim的基本操作

`:w`保存文件

`:q`退出

`:wq`保存并退出

`i` - 进入编辑模式

`esc` - 退出编辑模式

`h、j、k、l` - 移动光标

`vim 1.txt`建立一个新的`1.txt`



深入了解：

+ [Linux vi/vim](https://www.runoob.com/linux/linux-vim.html)



### Linux环境变量

`PATH`

`PKG_CONFIG_PATH`

`环境变量的变更`

linux环境 - `~/.bashrc`

mac - `~/.bash_profile`

设置好之后，需要执行`source`命令才可以真正的生效

**env命令**用于显示系统中已存在的环境变量

![002](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/002.png)

通过`grep`过滤，`env | grep PATH`

![003](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/003.png)

> PATH使用`:`分隔



>  一些说明：
>
> 现在mac下的shell默认为zsh，不再是bash
>
> 通过`echo $SHELL`可查看shell的版本
>
> 由`bash`切换到`zsh`后`.bash_profile`里面的环境变量的配置便不生效了，需要在`~/.zshrc`中重新配置



`vi ~/.zshrc`，本人电脑的配置如下：

![004](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/004.png)

课程视频截图如下：

![005](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/005.png)

> 可以看出，有多个`PATH`的设置，`$PATH`表示原来的`PATH`设置，意思是把原来的`PATH`设置给拼接过来

编辑完后，需要使用`source ~/.zshrc` 命令



## Mac或Linux下编译安装ffmpeg

![007](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/007.png)

1.`brew`方式

`brew search ffmpeg`搜索ffmpeg

![006](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/006.png)

`brew install ffmpeg`安装

2.源码的方式

在[官网](https://ffmpeg.org/download.html)，通过git下载

```shell
git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg
```



### 编译

1.`./configure --prefix=./ffmpeg --enable-debug=3`

可通过`./configure --help`来查看，支持的那些参数

可通过搜索来过滤

![008](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/008.png)

2.这里使用`./configure --prefix=$PWD/output --enable-debug=3 --enable-shared --disable-static`

启用动态库，禁用静态库

3.`make -j 4`

4.`make install`

编译后的输出结构如下：

![009](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/009.png)

![010](https://github.com/winfredzen/VideoAudio/blob/main/Basic/image/010.png)

5.设置PATH，我brew安装，就省略了























