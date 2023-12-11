#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define BUFFER_SIZE 64
#define MAX_EVENT_NUMBER 5

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd) 
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int main(int argc, char* argv[])
{
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", argv[0]);
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("connection failed\n");
        close(sockfd);
        return 1;
    }

    int epollfd;
    epoll_event events[MAX_EVENT_NUMBER];
    epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, 0);
    addfd(epollfd, sockfd);

    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);

    while (1) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; i++) {
            int connfd = events[i].data.fd;
            if (connfd == sockfd) {
                if (events[i].events & EPOLLRDHUP) {
                    printf("server close the connection\n");
                    break;
                }
                else if (events[i].events & EPOLLIN) {
                    memset(read_buf, '\0', BUFFER_SIZE);
                    while (1) {
                        ret = recv(connfd, read_buf, BUFFER_SIZE - 1, 0);
                        if (ret == -1) {
                            if (errno == EAGAIN) {
                                continue;
                            }
                            break;
                        }
                        else if (ret == 0) {
                            break;
                        }
                        printf("%s", read_buf);
                    }
                }
            }
            else if (connfd == 0) {
                if (events[i].events & EPOLLIN) {
                    ret = splice(0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
                    // printf("recv data from fd 0\n");
                    ret = splice(pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
                    // printf("send data to sokfd\n");
                }
            }
        }
    }

    close(sockfd);
    return 0;
}