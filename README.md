2023.11.24 
这是第一次接触webserver网络编程和多线程编程相关的项目，主要通过Linux高性能服务器编程这本书和从零开始自制webserver这个网上的blog进行相关的学习和实践。let's start！

由于之前没有相关的经验，仿照书上第5节和blog上的第一篇完成一个简单的回显设置：webecho.cpp
1.其中发现dprintf和send似乎区别不大，都可以用来发送数据？
2.仿照书上的6.4节简单的测试文件的传输；

第一个的webecho的回显程序有一个问题就是它是阻塞i/o，单个线程只能支持一个客户端；那么在第一个简单的webecho文件的基础上进行改进，参考书上第九章和blog第一节中的后续内容使用epoll这一I/O复用技术进行改进，最终完成了mulio这一程序：
1.实现的内容仍然是最简单的回显示；
2.相比于上一个程序，能够满足在单个进程中多个客户端同时访问；



2023.11.29
仿照书上和blog完成单进程/线程I/O复用的简单echo服务器后，后续的内容是仿照blog的第二节完成多线程/进程的web服务器；在粗略的看了blog之后大概明确了本次的任务，但是对于这种多线程/进程的编程之前还没有接触过，准备边看书便了解。那么就从书上的epoll继续吧！

blog上的内容直接到了多线程/进程编程，所以准备主要参考书上的代码学习到其中的多进程/线程编程，首先仿照第九章后面io多路复用的例子，完成了一个简易的聊天室应用：
1.支持多个用户端连接同一个服务器程序；
2.服务器程序接收不同客户端的内容并把内容分发给除发送端外的其他客户端机器；
仿照copy下来一个比较明显的直观感受是它在服务器端使用while的无限循环作为主体，在其中不停的遍历所有的连接，一旦连接的数量很大，可能速度会变的相当慢(因为，每次客户端发一个程序，服务器端在接受的同时都要遍历一遍其他连接的客户端，并把当前客户端发送的内容写入到其他客户端的缓存中去，连接数变多的话操作的数量要指数级增长)；

参考书上的第十章在前面的聊天室应用中仿照书上的统一事件源代码示例给聊天室加入了使用信号关闭的功能(虽然本来就有默认的进程中断，但不同的是通过处理，可以实现其中申请变量的销毁等)
1.书上的代码使用的是epoll，但个人觉得epoll虽然可以不用循环遍历fds数组，但在发送给其他连接时不太方便，不能像poll那样直接遍历，可能还需要维护一个专门的数组；所以最终是把书上的epoll改为了适用于poll的代码；
2.书上的代码实现的原理是当有一个信号过来时，通过信号处理程序直接发送一个信号，最终的处理手段与其他socket类似；
3.实际使用时存在bug，书上代码中的发送实际与接受哦结果不一致(未知晓原因)，改为其他方法后可以实现同样的方案；(改完存在一个小bug，在printf示例test时没有立即刷新出来)

参考书上的第十三章多进程编程，仿照13-4的代码写了一个利用共享内存的聊天室服务器程序，实现了最简单的聊天室功能，后续准备加上：
1.client的客户端改成和server相同的epoll；(实际编写的时候遇到了一个与epoll有关的LT和ET模式的问题，在LT模式下可以沿用就的poll的思路直接接收即可；但是在ET模式下由于每次发送只会触发一次，所以需要一次性接收完成所以需要一个while()循环来一次接收完成信息)
2.server服务器可以改成使用进程池；(主要是进程池类processpool和模板参数类，主要借鉴blog2进行仿写, 最终实现的是和blog 2相同的echo服务器程序)(目前暂时不能实现想要的chatroom，如果想要改成chatroom的话，需要更改processpool.h中的run_parent然后把chatroom稍加改进)；
3.学会使用makefile来编译管理文件；


