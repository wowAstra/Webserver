#ifndef MY_MUDUO_HTTPCONTENT_H_
#define MY_MUDUO_HTTPCONTENT_H_

#include <utility>
#include <algorithm>
#include "buffer.h"
#include "httprequest.h"
#include "httpparsestate.h"

namespace my_muduo {

enum HttpRequestParseLine {
    kLineOk,
    kLineMore,
    kLineErrno
};

class HttpContent {
public:
    HttpContent();
    ~HttpContent();

    void ParseLine(Buffer* buffer);
    bool ParseContent(Buffer* buffer);
    bool GetCompleteRequest() {return parse_state_ == kParseGotCompleteRequest;}

    const HttpRequest& request() const {return request_;}
    void ResetContentState() {
        parse_state_ = kParseRequestLine;
        line_state_ = kLineOk;
    }

private:
    int checked_index_;
    HttpRequest request_;
    HttpRequestParseLine line_state_;
    HttpRequestParseState parse_state_;
};

}

#endif