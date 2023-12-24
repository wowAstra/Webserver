#ifndef MY_MUDUO_THREAD_H_
#define MY_MUDUO_THREAD_H_

#include <pthread.h>
#include <functional>

#include "latch.h"

namespace my_muduo {

class Thread {
public:
    typedef std::function<void()> ThreadFunc;

    Thread(const ThreadFunc& func);
    ~Thread();

    void StartThread();

private:
    pthread_t pthread_id_;
    ThreadFunc func_;
    Latch latch_;
};

struct ThreadData {

typedef my_muduo::Thread::ThreadFunc ThreadFunc;

ThreadFunc func_;
Latch* latch_;

ThreadData(ThreadFunc& func, Latch* latch)
    : func_(func), latch_(latch) {
}

void RunOneFunc() {
    latch_->CountDown();
    latch_ = nullptr;
    func_();
}
};

}

#endif