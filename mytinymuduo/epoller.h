#ifndef MY_MUDUO_EPOLLER_H
#define MY_MUDUO_EPOLLER_H

#include <sys/epoll.h>
#include <vector>
#include <map>

#include "noncopyable.h"

static const int kDefaultEvents = 16;


namespace my_muduo {

class Channel;

class Epoller /*: public NonCopyAble*/ {
public:
    typedef std::vector<epoll_event> Events;
    typedef std::vector<Channel*> Channels;

    Epoller();
    ~Epoller();

    void Remove(Channel* channel_);
    void Poll(Channels& channels);
    int EpollWait() {return epoll_wait(epollfd_, &*events_.begin(), 
                                       static_cast<int>(events_.size()), -1);}
    void FillActiveChannels(int eventnums, Channels& channels);
    void Update(Channel* channel);
    void UpdateChannel(int operation, Channel* channel);

private:
    typedef std::map<int, Channel*> ChannelMap;

    int epollfd_;
    Events events_;
    ChannelMap channels_;
};

}

#endif