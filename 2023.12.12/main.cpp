#include "echo.h"
#include "address.h"

using namespace my_muduo;

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printf("usage: ip port \n");
        return 0;
    }
    EventLoop loop;
    Address address(argv[1], argv[2]);
    EchoServer server(&loop, address);
    server.Start();
    loop.Loop();
    return 0;
}