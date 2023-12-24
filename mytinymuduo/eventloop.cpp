#include "eventloop.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <pthread.h>

#include "mutex.h"
#include "channel.h"

namespace my_muduo {
EventLoop::EventLoop()
    : tid_(::pthread_self()), epoller_(new Epoller()), wakeup_fd_(::eventfd(0, EFD_NONBLOCK)),
      wakeup_channel_(new Channel(this, wakeup_fd_)) {
    wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    wakeup_channel_->EnableReading();
}

EventLoop::~EventLoop() {}

void EventLoop::Loop() {
    while (1) {
        epoller_->Poll(active_channels_);
        for (const auto& channel : active_channels_) {
            channel->HandleEvent();
        }
        active_channels_.clear();
        DoToDoList();
    }
}

void EventLoop::HandleRead() {
    uint64_t read_one_byte = 1;
    ::read(wakeup_fd_, &read_one_byte, sizeof(read_one_byte));
    return;
}

void EventLoop::RunOneFunc(const BasicFunc& func) {
    if (IsInThreadLoop()) {
        func();
    }
    else {
        {
            MutexLockGuard lock(mutex_);
            to_do_list_.emplace_back(func);
        }
        if (!IsInThreadLoop()) {
            uint64_t write_one_byte = 1;
            ::write(wakeup_fd_, &write_one_byte, sizeof(write_one_byte));
        }
    }
}

void EventLoop::DoToDoList() {
    ToDoList functors;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(to_do_list_);
    }

    for (const auto& func : functors) {
        func();
    }
}

}