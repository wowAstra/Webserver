#ifndef MY_MUDUO_TCPSERVER_H_
#define MY_MUDUO_TCPSERVER_H_

#include <memory>
#include <map>
#include <utility>
#include <string>

#include "callback.h"
#include "eventloop.h"
#include "acceptor.h"
#include "eventloopthreadpool.h"
#include "tcpconnection.h"
#include "noncopyable.h"
#include "logging.h"
#include "address.h"

namespace my_muduo {

class Address;

class TcpServer : public NonCopyAble {
public:
    TcpServer(EventLoop* loop, const Address& address);
    ~TcpServer();

    void Start() {
        threads_->StartLoop();
        loop_->RunOneFunc(std::bind(&Acceptor::Listen, acceptor_.get()));
    }

    void SetConnectionCallback(const ConnectionCallback& callback) {
        connection_callback_ = callback;
    }

    void SetConnectionCallback(ConnectionCallback&& callback) {
        connection_callback_ = std::move(callback);
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void SetMessageCallback(MessageCallback&& callback) {
        message_callback_ = std::move(callback);
    }

    void SetThreadNums(int thread_nums) {
        threads_->SetThreadNums(thread_nums);
    }

    void HandleClose(const TcpConnectionPtr& conn);
    void HanldeCloseInLoop(const TcpConnectionPtr& ptr);
    void HandleNewConnection(int connfd, const Address& address);

private:
    typedef std::map<int, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    int next_connection_id_;
    std::unique_ptr<EventLoopThreadPool> threads_;
    std::unique_ptr<Acceptor> acceptor_;
    const std::string ipport_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    ConnectionMap connections_;
};

}

#endif