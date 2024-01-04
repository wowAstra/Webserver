#ifndef MY_MUDUO_HTTPSERVER_H_
#define MY_MUDUO_HTTPSERVER_H_

#include <stdio.h>

#include <functional>
#include <utility>
#include <memory>

#include "callback.h"
#include "eventloop.h"
#include "tcpserver.h"
#include "tcpconnection.h"
#include "buffer.h"
#include "httpcontent.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "timestamp.h"

using my_muduo::HttpStatusCode;

namespace my_muduo {

static const double kIdleConnectionTimeouts = 8.0;
static const int kThreadNums = 8;

class EventLoop;

class HttpServer {
typedef std::function<void (const HttpRequest&, HttpResponse&)> HttpResponseCallback;

public:
    HttpServer(EventLoop* loop, const Address& address, bool auto_close_idleconnction);
    ~HttpServer();

    void test() const {
        printf("%s test for timer\n", Timestamp::Now().ToFormattedString().data());
    }

    void Start() {
        server_.Start();
    }

    void HttpDefaultCallback(const HttpRequest& request, HttpResponse& response) {
        response.SetStatusCode(k404NotFound);
        response.SetStatusMessage("Not Found");
        response.SetCloseConnection(true);
    }

    void HandleIdleConnection(std::weak_ptr<TcpConnection>& connection) {
        TcpConnectionPtr conn(connection.lock());
        if (conn) {
            if (Timestamp::AddTime(conn->timestamp(), kIdleConnectionTimeouts) < Timestamp::Now()) {
                // printf("%s HttpServer::HandleIdleConnection close connection\n", Timestamp::Now().ToFormattedString().data());
                conn->Shutdown();
            }
            else {
                loop_->RunAfter(kIdleConnectionTimeouts, std::bind(&HttpServer::HandleIdleConnection, this, connection));
            }
        }
    }

    void ConnectionCallback(const TcpConnectionPtr& connection) {
        if (auto_close_idleconnection_) {
            // printf("%s HttpServer::ConnectionCallback\n", Timestamp::Now().ToFormattedString().data());
            loop_->RunAfter(kIdleConnectionTimeouts, std::bind(&HttpServer::HandleIdleConnection, this, std::weak_ptr<TcpConnection>(connection)));
        }
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
    bool auto_close_idleconnection_;

    HttpResponseCallback response_callback_;
};
}

#endif