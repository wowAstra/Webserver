#ifndef MY_MUDUO_EVENTLOOP_H_
#define MY_MUDUO_EVENTLOOP_H_

#include <vector>
#include <functional>

#include "epoller.h"

namespace my_muduo {
    
class Epoller;
class Channel;

class EventLoop {
public:
    typedef std::vector<Channel*> Channels;
    typedef std::function<void()> BasicFunc;

    EventLoop();
    ~EventLoop();

    void Loop();
    void Update(Channel* channel) {epoller_->Update(channel);}
    void RunOneFunc(const BasicFunc& func) {func();}

private:
    Epoller* epoller_;
    int wakeup_fd_;
    Channels active_channels_;
};

}


#endif