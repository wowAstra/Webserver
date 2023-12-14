#ifndef MY_MUDUO_ADDRESS_H_
#define MY_MUDUO_ADDRESS_H_

#include <string>
#include <cstring>

namespace my_muduo {

class Address {
private:
    const char* ip_;
    const int port_;

public:
    Address(const char* ip = "127.0.0.1", const char* port = "1142") : ip_(ip), port_(atoi(port)) {}

    const char* ip() const {return ip_;}
    const int port() const {return port_;}
};

}

#endif 