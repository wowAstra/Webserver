#include "eventloop.h"
#include "channel.h"

namespace my_muduo {
EventLoop::EventLoop()
    : epoller_(new Epoller()), wakeup_fd_(0) {
}

EventLoop::~EventLoop() {}

void EventLoop::Loop() {
    while (1) {
        epoller_->Poll(active_channels_);
        int size = active_channels_.size();
        printf("EventLoop Loop:: eventnum %d\n", size);
        for (const auto& channel : active_channels_) {
            printf("EventLoop Loop connfd %d\n", channel->fd());
            channel->HandleEvent();
        }
        active_channels_.clear();
    }
}

}