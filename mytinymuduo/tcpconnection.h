#ifndef MY_MUDUO_TCPCONNECTION_H_
#define MY_MUDUO_TCPCONNECTION_H_

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

#include "callback.h"
#include "channel.h"
#include "buffer.h"
#include "httpcontent.h"

using std::string;

namespace my_muduo {
    
class EventLoop;

class TcpConnection {
public:
    TcpConnection(EventLoop* loop, int connfd);
    ~TcpConnection();

    void SetConnectionCallback(const ConnectionCallback& callback) {
        connection_callback_ = callback;
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void ConnectionEstablished() {
        channel_->EnableReading();
        connection_callback_(this, &input_buffer_);
    }

    HttpContent* GetHttpContent() {
        return &content_;
    }

    void Shutdown() {
        if (!channel_->IsWriting()) {
            shutdown_ = true;
            ::shutdown(fd_, SHUT_WR);
        }
    }

    bool IsShutdown() const {return shutdown_;}
    void HandleMessage();
    void HandleWrite();
    void Send(Buffer* buffer);
    void Send(const string& str);
    void Send(const char* message, int len);
    void Send(const char* message) {Send(message, strlen(message));}

private:
    EventLoop* loop_;
    int fd_;
    bool shutdown_;
    Channel* channel_;
    Buffer input_buffer_;
    Buffer output_buffer_;
    HttpContent content_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
};

}



#endif 