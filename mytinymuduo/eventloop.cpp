#include "eventloop.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <pthread.h>
#include <signal.h>

#include <utility>
#include <assert.h>

#include "mutex.h"
#include "channel.h"

using namespace my_muduo;

namespace {

class IgnoreSigPipe {
public:
    IgnoreSigPipe() {
        ::signal(SIGPIPE, SIG_IGN);
    }
};

IgnoreSigPipe initObj;

}

EventLoop::EventLoop()
    : running_(false),
      tid_(::pthread_self()), 
      epoller_(new Epoller()), 
      wakeup_fd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
      wakeup_channel_(new Channel(this, wakeup_fd_)), 
      timer_queue_(new TimerQueue(this)),
      calling_functors_(false) {
    wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    wakeup_channel_->EnableReading();
}

EventLoop::~EventLoop() {
    if (running_) running_ = false;
    wakeup_channel_->DisableAll();
    Remove(wakeup_channel_.get());
    close(wakeup_fd_);
}

void EventLoop::Loop() {
    assert(IsInThreadLoop());
    running_ = true;
    while (1) {
        active_channels_.clear();
        epoller_->Poll(active_channels_);
        for (const auto& channel : active_channels_) {
            channel->HandleEvent();
        }
        DoToDoList();
    }
    running_ = false;
}

void EventLoop::HandleRead() {
    uint64_t read_one_byte = 1;
    ssize_t read_size = ::read(wakeup_fd_, &read_one_byte, sizeof(read_one_byte));
    (void) read_size;
    assert(read_size == sizeof(read_one_byte));
    return;
}

void EventLoop::QueueOneFunc(BasicFunc func) {
    {
        MutexLockGuard lock(mutex_);
        to_do_list_.emplace_back(std::move(func));
    }

    if (!IsInThreadLoop() || calling_functors_) {
        uint64_t write_one_byte = 1;
        int write_byte = ::write(wakeup_fd_, &write_one_byte, sizeof(write_one_byte));
        (void) write_byte;
        assert(write_byte == sizeof(write_one_byte));
    }
}

void EventLoop::RunOneFunc(BasicFunc func) {
    if (IsInThreadLoop()) {
        func();
    }
    else {
        QueueOneFunc(std::move(func));
    }
}

void EventLoop::DoToDoList() {
    ToDoList functors;
    calling_functors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(to_do_list_);
    }

    for (const auto& func : functors) {
        func();
    }
    calling_functors_ = false;
}
