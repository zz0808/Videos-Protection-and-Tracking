#### 估算固定摄像头的视频的背景

对于采集的视频，随机采取25帧，对于每个像素点，计算这25张图像对应像素点的中位数，作为背景对应的像素点。

视频截图：

<img src=".\sample.jpg" alt="sample" style="zoom:33%;" />

估计到的背景：

<img src=".\frame.jpg" alt="frame" style="zoom: 33%;" />

结果截图：

<img src=".\ret.jpg" alt="ret" style="zoom:33%;" />