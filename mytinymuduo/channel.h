#ifndef MY_MUDUO_CHANNEL_H_
#define MY_MUDUO_CHANNEL_H_

#include <sys/epoll.h>

#include "eventloop.h"
#include "callback.h"

namespace my_muduo {
    
class Channel {
public:
    Channel(EventLoop* loop, const int& fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallback(const ReadCallback& callback) {
        read_callback_ = callback;
    }

    void SetWriteCallback(const WriteCallback& callback) {
        write_callback_ = callback;
    }

    void EnableReading() {
        events_ |= EPOLLIN;
        Update();
    }

    void EnableWriting() {
        events_ |= EPOLLOUT;
        Update();
    }

    void Update() {
        loop_->Update(this);
    }

    void SetReceiveEvents(int events) {
        recv_events_ = events;
    }

    int fd() {return fd_;}
    int events() {return events_;}
    int recv_events() {return recv_events_;}

private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int recv_events_;

    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

}

#endif 