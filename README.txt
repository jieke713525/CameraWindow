该项目仅仅针对相机的Gain，AcquisitionFrameRate，ExposureTime三者进行了属性的get/set操作，更多的相关操作可以从官网获取

图像的单拍功能较为简单，连拍功能使用QThread+信号槽的方式来完成

图像的保存功能还没有完善，基本思路就是QImage+FileDialog的方式

Basler相机的属性设置基本上没有问题，Native和Generic的方式都会，只要照着官网就可以

Basler相机的事件注册和使用也基本掌握，但是用的不多，本人的理解就是：异步操作+代码解耦

Basler相机的图像抓取目前已经熟悉大致的流程，StartGrabbing-->RetrieveResult-->buffer的处理

因为buffer的处理涉及到和像素矩阵有关的一系列操作，本人还没有接触过，所以采用CPylonImage进行了简化的操作

这是本人目前针对该Demo的总结，有不足之处还望指正

名为demo的视频大致演示了Demo的运行流程