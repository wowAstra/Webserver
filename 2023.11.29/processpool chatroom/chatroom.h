#ifndef __CHATROOM_H_
#define __CHATROOM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "processpool.h"

class chatroom 
{
private:
    static const int BUFFER_SIZE = 1024;
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFER_SIZE];

public:
    chatroom(){}
    ~chatroom(){}

    void init(int epollfd, int sockfd, const sockaddr_in& client_addr)
    {
        m_epollfd = epollfd;
        m_sockfd = sockfd;
        m_address = client_addr;
        memset(m_buf, '\0', BUFFER_SIZE);
    }

    void process()
    {
        int ret = -1;
        while (true) {
            memset(m_buf, '\0', BUFFER_SIZE);
            ret = recv(m_sockfd, m_buf, BUFFER_SIZE - 1, 0);
            if (ret < 0)  {
                if (errno != EAGAIN) {
                    removefd(m_epollfd, m_sockfd);
                }
                break;
            }
            else if (ret == 0) {
                removefd(m_epollfd, m_sockfd);
                break;
            }
            else {
                send(m_sockfd, m_buf, BUFFER_SIZE - 1, 0);
            }
        }
        return;
    }
};

int chatroom::m_epollfd = -1;

#endif