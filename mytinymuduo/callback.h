#ifndef MY_MUDUO_CALLBACK_H_
#define MY_MUDUO_CALLBACK_H_

#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace my_muduo {
    class TcpConnection;
    class Buffer;
    
    typedef std::function<void(TcpConnection*, Buffer*)> ConnectionCallback;
    typedef std::function<void(TcpConnection*, Buffer*)> MessageCallback;
    typedef std::function<void()> ReadCallback;
    typedef std::function<void()> WriteCallback;
}
#endif