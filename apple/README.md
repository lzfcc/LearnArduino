# Bad Apple 播放

## 串口通信

### 步骤

1. 下载 Bad Apple 视频

2. 下载 [ffmpeg](https://www.ffmpeg.org/)并处理视频

将视频尺寸缩小。原视频比例为 4 : 3，M5StickC 屏幕为 160 x 80，所以选择将视频尺寸缩小为 96 x 72，正好都是整字节数。

然后将视频按照 6 帧/秒提取帧。(这个选择没有什么依据，理论上越大播放越流畅，但是受限于屏幕的刷新频率)
```shell
$ ffmpeg -i badapple.mp4 -s 96x72 smallapple.mp4 
$ ffmpeg -i smallapple.mp4 -r 6 image-%04d.jpg
```

3. 下载 image2lcd（Windows）将图片转为二进制

注意勾选“字节内像素数据反序”，不要勾选“包含图像头数据”，否则显式异常。如果将数据写在闪存里（`setup()`时），输出数据类型适合于勾选“C 语言数组(.c)”（在 Arduino 中用 .h 文件引入），否则勾选“二进制(.bin)”。

4. 编写 Arduino 程序

仿照 M5StickC 示例中的 Advanced -> Display -> DrawXBitmap。注意这里并非使用`drawBitmap`而是`drawXBitMap`。前者可以绘制 16 位深的彩色图像，而后者是用来绘制二值图像的（X Bitmap）。

[X Bitmap](https://en.wikipedia.org/wiki/X_BitMap)是一种很古早的图像格式，它本身实际上就是一段 C 代码（图像宽高和数据的一维数组定义）。它用 1 个 bit 代表 1 个像素，所以只有 0/1 两个值。位于图像左上角的像素在数组中用第 1 个元素的最低位表示。（所以要勾选“字节内像素反序”）

`setup()`中绘制一些静态的装饰性内容，`loop()`中绘制串口数据。

5. 编写串口通信程序（communication.py）

使用 python 2.7 和 serial（需要安装）。按顺序读取图像二进制文件然后发送到串口即可。

### 存在的问题

1. 画面闪动频繁

2. 必须通过串口通信，无法脱离电脑

### 解决方案

1. “局部更新”，计算相邻两帧图像的差异（异或操作）

2. 压缩视频