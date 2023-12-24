#ifndef MY_MUDUO_ECHO_H_
#define MY_MUDUO_ECHO_H_

#include <stdio.h>
#include <string>

#include "tcpserver.h"
#include "tcpconnection.h"

static const int thread_nums = 8;

namespace my_muduo {
    class Address;
    class EventLoop;
}

class EchoServer {
public:
    EchoServer(my_muduo::EventLoop* loop, const my_muduo::Address& listen_addr);
    ~EchoServer() {};

    void Start() {
        server_.Start();
    }

    void ConnectionCallback(my_muduo::TcpConnection* connection) {
        printf("server has a new connection. \n");
    }

    void MessageCallback(my_muduo::TcpConnection* connection) {
        std::string msg(connection->Get());
        printf("server get a message. \n");
        connection->Send(msg);
    }

private:
    my_muduo::EventLoop* loop_;
    my_muduo::TcpServer server_;
};

#endif