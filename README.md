2023.11.24 
这是第一次接触webserver网络编程和多线程编程相关的项目，主要通过Linux高性能服务器编程这本书和从零开始自制webserver这个网上的blog进行相关的学习和实践。let's start！

由于之前没有相关的经验，仿照书上第5节和blog上的第一篇完成一个简单的回显设置：webecho.cpp
1.其中发现dprintf和send似乎区别不大，都可以用来发送数据？
2.仿照书上的6.4节简单的测试文件的传输；

第一个的webecho的回显程序有一个问题就是它是阻塞i/o，单个线程只能支持一个客户端；那么在第一个简单的webecho文件的基础上进行改进，参考书上第九章和blog第一节中的后续内容使用epoll这一I/O复用技术进行改进，最终完成了mulio这一程序：
1.实现的内容仍然是最简单的回显示；
2.相比于上一个程序，能够满足在单个进程中多个客户端同时访问；
