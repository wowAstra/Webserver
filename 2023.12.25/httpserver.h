#ifndef MY_MUDUO_HTTPSERVER_H_
#define MY_MUDUO_HTTPSERVER_H_

#include <stdio.h>

#include <functional>
#include <utility>

#include "callback.h"
#include "tcpserver.h"
#include "tcpconnection.h"
#include "buffer.h"
#include "httpcontent.h"
#include "httprequest.h"
#include "httpresponse.h"

using my_muduo::HttpStatusCode;

namespace my_muduo {

static const int kThreadNums = 8;

class EventLoop;

class HttpServer {
typedef std::function<void (const HttpRequest&, HttpResponse&)> HttpResponseCallback;

public:
    HttpServer(EventLoop* loop, const Address& address);
    ~HttpServer();

    void Start() {
        server_.Start();
    }

    void HttpDefaultCallback(const HttpRequest& request, HttpResponse& response) {
        response.SetStatusCode(k404NotFound);
        response.SetStatusMessage("Not Found");
        response.SetCloseConnection(true);
    }

    void ConnectionCallback(const TcpConnectionPtr& connection) {
    }

    void MessageCallback(const TcpConnectionPtr& connection, Buffer* buffer);

    void SetHttpResponseCallback(const HttpResponseCallback& response_callback) {
        response_callback_ = response_callback;
    }

    void SetHttpResponseCallback(HttpResponseCallback&& response_callback) {
        response_callback_ = std::move(response_callback);
    }

    void DealWithRequest(const HttpRequest& request, const TcpConnectionPtr& connection);

private:
    EventLoop* loop_;
    TcpServer server_;

    HttpResponseCallback response_callback_;
};
}

#endif