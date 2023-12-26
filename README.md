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


2023.12.12
参考blog的内容，准备简单的使用muduo库的思路建立一个简单的echo服务器；
12.13:简单的了解muduo网络库了解了各个文件(类)的作用，把blog上的简易echo服务器实现,了解了一些makefile的编写规则：
1.开始时无法正常工作，实际具体梳理时发现是把epoll_create打成了epoll_createl，要注意细节问题；
2.$^ $@ $< 分别是所有的依赖文件，目标文件和第一个依赖文件；
3.由于将网络库的内容单独放在了外面的文件夹，所以需要
    INCLUDES = -I ../mytinymuduo  $(INCLUDES) 
这个指令来指明依赖的文件路径，相应的下面的编译中也要加入$(INCLUDES)选项
    VPATH = src:../mytinymuduo
vpath用来指定makefile的搜索路径； 
4.make完成后的所有中间文件都会放在原有的文件夹，使用
    -o $(OBJ_DIR)$@
指定中间文件的存储路径，这是比较简单的一种存储方式，后续可以改进；在改变文件的存储位置后，main这一可执行文件的链接会出现问题，因为makefile在本地的工作pwd中寻找中间文件，需要在makefile的依赖路径中加入存储中间文件的文件夹；


2023.12.15
向原有的基于muduo库的echo回显服务器中加入了多线程相关的内容：
1.主要是eventloopthread相关的类，以及一些线程锁相关的前置内容；
2.之前对makefile的编写规则理解有误，文件的生成项中自带了其存放位置，直接修改文件名就可以更改；
3.vpath的主要功能是向makefile加入依赖库，而如果更改了文件存放的位置，那web_server这一可执行文件的依赖文件中就不能是原名，因为原名只会在当前路径中查询文件，一种方案是将生成的文件的文件夹加入vpath中，更一般的方案是直接把依赖文件改称带有具体位置的文件名；


2023.12.18
有了简单的多线程服务器后向其中加入buffer缓冲机制和http状态机相关的内容:
1.buffer机制有buffer.h和buffer.cpp两个文件，里面主要利用的是vector数组；
2.加入了处理http报文的http服务器；

主要是要明白muduo库中复杂的回调机制和bind相关的内容，回调机制可以让模块专注于自己的功能而不必在意该功能在何时何地实现；以最初的echo服务器为例，我们在echo服务器中有tcpserver类，以及自己的两个回调函数(concallback, messcallback),我们在构造函数时通过echo类中的tcpserver类来注册这两个回调函数；这样我们之前实现的两个函数就实际上保存在了tcpserver类中，其他的回调函数也类似，这样的话就完成了函数的重复使用，并且不需要继承等其他手法，只需要类中存在相应的空间以保存这些函数和有关的类；


2023.12.25
参考blog调试，问题：
1.为什么不论是muduo库，还是这里的webserver都不调用子线程？
2.为什么要用::write代替原来的send；
3.shared_from_this是什么；
4.为什么原来的直接赋值变成了std::move()，以及相应的callback中的函数形式为什么要改成现在这样；