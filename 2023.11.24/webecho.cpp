#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <bits/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>


int main(int argc, char* argv[])
{
    if (argc <= 2) {
        printf("Usage : %s ip_address port", argv[0]);
        return 0;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &my_addr.sin_addr);
    my_addr.sin_port = htons(port);

    int ret = bind(listenfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int acceptfd = accept(listenfd, (struct sockaddr *)&client, &client_addrlength);
    assert(acceptfd != -1);
    
    // web回显,这是blog上的最简易的回显设置
    // char buf_size[1024] = {0};
    // int recv_size = 0;
    // recv_size = recv(acceptfd, buf_size, sizeof(buf_size), 0);

    // int send_size = 0;
    // const char* buff = "hello, web!";
    // // printf("size of buff is %ld\n", sizeof(buff));
    // // printf("size of pointer is %ld\n", sizeof(char *));
    // // printf("size of integer is %ld\n", sizeof(int));
    // send_size = send(acceptfd, buff, 11, 0);

    // // dprintf(acceptfd, "hello web!\n");


    // 对于sendfile的测试
    // const char* file_name = argv[3];
    // int filefd = open(file_name, O_RDONLY);
    // struct stat stat_buf;
    // fstat(filefd, &stat_buf);
    // sendfile(acceptfd, filefd, NULL, stat_buf.st_size);


    //书上第六章中新的回显方式，比起前面的好处是，他无需内核到用户的数据拷贝
    int pipefd[2];
    ret = pipe(pipefd);
    assert(ret != -1);
    ret = splice(acceptfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);
    ret = splice(pipefd[0], NULL, acceptfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);
    

    close(listenfd);
    close(acceptfd);

    return 0;
}