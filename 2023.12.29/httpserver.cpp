#include "httpserver.h"

#include <functional>

using namespace my_muduo;

using my_muduo::Version;

HttpServer::HttpServer(EventLoop* loop, const Address& address, bool auto_close_idleconnection) 
    : loop_(loop), 
      server_(loop, address),
      auto_close_idleconnection_(auto_close_idleconnection) {
    server_.SetConnectionCallback(std::bind(&HttpServer::ConnectionCallback, this, _1));
    server_.SetMessageCallback(std::bind(&HttpServer::MessageCallback, this, _1, _2));
    server_.SetThreadNums(kThreadNums);
    SetHttpResponseCallback(std::bind(&HttpServer::HttpDefaultCallback, this, _1, _2));
}

HttpServer::~HttpServer() {}

void HttpServer::MessageCallback(const TcpConnectionPtr& connection, 
                                 Buffer* buffer) {
    HttpContent* content = connection->GetHttpContent();
    if (auto_close_idleconnection_) {
        connection->UpdateTimestamp(Timestamp::Now());
        printf("%s HttpServer::MessageCallback\n", Timestamp::Now().ToFormattedString().data());
    }
    if (connection->IsShutdown()) return;
    
    if (!content->ParseContent(buffer)) {
        connection->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
        connection->Shutdown();
    }

    if (content->GetCompleteRequest()) {
        DealWithRequest(content->request(), connection);
        content->ResetContentState();
    }
}

void HttpServer::DealWithRequest(const HttpRequest& request, 
                                 const TcpConnectionPtr& connection) {
    string connection_state = std::move(request.GetHeader("Connection"));
    bool close = (connection_state == "Close" || 
                 (request.version() == kHttp10 && 
                  connection_state != "Keep-Alive"));
    
    HttpResponse response(close);
    response_callback_(request, response);
    Buffer buffer;
    response.AppendToBuffer(&buffer);
    connection->Send(&buffer);

    if (response.CloseConnection()) {
        connection->Shutdown();
    }
}
