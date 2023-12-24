#ifndef MY_MUDUO_TCPSERVER_H_
#define MY_MUDUO_TCPSERVER_H_

#include "callback.h"
#include "eventloop.h"
#include "acceptor.h"
#include "eventloopthreadpool.h"

namespace my_muduo {
class Address;
class EventLoopThreadPool;

class TcpServer {
public:
    TcpServer(EventLoop* loop, const Address& address);
    ~TcpServer();

    void Start() {
        threads_->StartLoop();
        loop_->RunOneFunc(std::bind(&Acceptor::Listen, acceptor_));
    }

    void SetConnectionCallback(const ConnectionCallback& callback) {
        connection_callback_ = callback;
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void SetThreadNums(int thread_nums) {
        threads_->SetThreadNums(thread_nums);
    }

    void NewConnection(int connfd);

private:
    EventLoop* loop_;
    EventLoopThreadPool* threads_;
    Acceptor* acceptor_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
};

}

#endif