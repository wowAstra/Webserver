#ifndef MY_MUDUO_CHANNEL_H_
#define MY_MUDUO_CHANNEL_H_

#include <sys/epoll.h>

#include "eventloop.h"
#include "callback.h"

namespace my_muduo {

enum ChannelState {
    kNew,
    kAdded,
    kDeleted
};
    
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

    void DisableWriting() {
        events_ &= ~EPOLLOUT;
        Update();
    }

    void RemoveFd() {

    }

    void Update() {
        loop_->Update(this);
    }

    void SetReceiveEvents(int events) {
        recv_events_ = events;
    }

    void SetChannelState(ChannelState state) {
        state_ = state;
    }

    int fd() {return fd_;}
    int events() {return events_;}
    int recv_events() {return recv_events_;}
    ChannelState state() {return state_;}

    bool IsWriting() {return events_ & EPOLLOUT;}
    bool IsReading() {return events_ & EPOLLIN;}

private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int recv_events_;

    ChannelState state_;
    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

}

#endif 