#ifndef MY_MUDUO_HTTPPARSESTATE_H_
#define MY_MUDUO_HTTPPARSESTATE_H_

namespace my_muduo {

enum HttpRequestParseState {
    kParseRequestLine,
    kParseHeaders,
    kParseBody,
    kParseGotCompleteRequest,
    kParseErrno,
};

}
#endif