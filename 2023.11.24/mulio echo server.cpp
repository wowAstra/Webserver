// 这个程序依旧完成的是回显任务，不过与上一个文件不同的是，
// 参考书上的第九章和blog第一节后续的内容简单的引入了I/O复用功能（即epoll）
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

#define MAX_EVENTS_NUMBER 5

int setnonblocking(int fd) 
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events =EPOLLIN;
    event.events |= EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
    return;
}

int main(int argc, char* argv[])
{
    if (argc <= 2) {
        printf("Usage: %s ip_address port_number\n", argv[0]);
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int ret = 0;
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENTS_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd);

    while (1) {
        int number = epoll_wait(epollfd, events, MAX_EVENTS_NUMBER, -1);
        if (number < 0) {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                addfd(epollfd, connfd);
            }
            else if (events[i].events & EPOLLIN) {
                char buf[1024];
                printf("event trigger once\n");
                while (1) {
                    memset(buf, '\0', sizeof(buf));
                    int ret = recv(sockfd, buf, sizeof(buf) - 1, 0);
                    if (ret < 0) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            printf("read later\n");
                            break;
                        }
                        close(sockfd);
                        break;
                    }
                    else if (ret == 0) {
                        close(sockfd);
                    }
                    else {
                        send(sockfd, buf, ret, 0);
                    }
                }
            }
            else {
                printf("something else happened\n");
            }
        }
    }
    close(listenfd);
    return 0;
}