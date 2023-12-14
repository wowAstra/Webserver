#ifndef MY_MUDUO_CALLBACK_H_
#define MY_MUDUO_CALLBACK_H_

#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace my_muduo {
    class TcpConnectionPtr;
    typedef std::function<void(TcpConnectionPtr*)> ConnectionCallback;
    typedef std::function<void(TcpConnectionPtr*)> MessageCallback;
    typedef std::function<void()> ReadCallback;
    typedef std::function<void()> WriteCallback;
}
#endif