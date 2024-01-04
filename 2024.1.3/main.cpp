#include <stdio.h>

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <climits>

#include "eventloop.h"
#include "address.h"
#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpresponsefile.h"
#include "logging.h"
#include "asynclogging.h"

using namespace my_muduo;

using my_muduo::Method;
using my_muduo::HttpStatusCode;

std::unique_ptr<AsyncLogging> asynclog;
extern void SetOutputFunc(Logger::OutputFunc);
extern void SetFlushFunc(Logger::FlushFunc);

void AsyncOutputFunc(const char* data, int len) {
    asynclog->Append(data, len);
}

void AsyncFlushFunc() {
    asynclog->Flush();
}

void HttpResponseCallback(const HttpRequest& request, HttpResponse& response) {
    if (request.method() == kGet) {
        const string& path = request.path();
        if (path == "/") {
            response.SetStatusCode(k200OK);
            response.SetBodyType("text/html");
            response.SetBody(love6_website);
        }
        else if (path == "/hello") {
            response.SetStatusCode(k200OK);
            response.SetBodyType("text/plain");
            response.SetBody("Hello, Website!\n");
        }
        else if (path == "/favicon") {
            response.SetStatusCode(k200OK);
            response.SetBodyType("image/png");
            response.SetBody(string(favicon, sizeof(favicon)));
        }
        else {
            response.SetStatusCode(k404NotFound);
            response.SetStatusMessage("Not Found");
            response.SetBody("Sorry, Not Found\n");
            response.SetCloseConnection(true);
            return;
        }
    }
    else if (request.method() == kPost){
        response.SetStatusCode(k200OK);
        response.SetBodyType("text/plain");
        response.SetBody("还没做\n");
        response.SetCloseConnection(true);
    }
    else {
        response.SetStatusCode(k400BadRequset);
        response.SetStatusMessage("Bad Request");
        response.SetCloseConnection(true);
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printf("usage: ip port \n");
        return 0;
    }

    // asynclog.reset(new AsyncLogging());
    // SetOutputFunc(AsyncOutputFunc);
    // SetFlushFunc(AsyncFlushFunc);
    // asynclog->StartAsyncLogging();

    EventLoop loop;
    Address address(argv[1], argv[2]);
    HttpServer server(&loop, address);
    server.SetThreadNums(8);
    server.SetHttpResponseCallback(HttpResponseCallback);
    server.Start();
    loop.Loop();
    return 0;
}