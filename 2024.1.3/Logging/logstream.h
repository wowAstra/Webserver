#ifndef MY_MUDUO_LOGSTREAM_H_
#define MY_MUDUO_LOGSTREAM_H_

#include <stdio.h>
#include <string.h>

#include <string>
#include <algorithm>

#include "noncopyable.h"

namespace my_muduo {

const int kSmallSize = 4096;
const int kLargeSize = 4096 * 1000;
static const int kNum2MaxStringLength = 48;
static const char digits[] = {'9', '8', '7', '6', '5', '4', '3', '2', '1','0',
                              '1', '2', '3', '4', '5', '6', '7', '8', '9'};

class GeneralTemplate : public NonCopyAble {
public:
    GeneralTemplate() : data_(nullptr), len_(0) {}
    explicit GeneralTemplate(const char* data, int len) : data_(data), len_(len) {}

    const char* data_;
    int len_;
};

template <int SIZE>
class FixedBuffer : public NonCopyAble {
public:
    FixedBuffer();
    ~FixedBuffer();

    static void CookieStart();
    static void CookieEnd();

    void SetCookie(void (*cookie)()) {cookie_ = cookie;}

    void Append(const char* data, int len) {
        if (writablebytes() < len) {
            len = writablebytes();
        }
        
        memcpy(cur_, data, len);
        cur_ += len;
    }

    void SetBufferZero() {
        memset(buf_, '\0', sizeof(buf_));
        cur_ = buf_;
    }

    void Add(int len) {
        cur_ += len;
    }

    const char* data() const {return buf_;}
    int len() const {return static_cast<int>(cur_ - buf_);}
    int writablebytes() const {return static_cast<int>(end() - cur_);}
    char* current() {return cur_;}
    const char* end() const {return buf_ + sizeof(buf_);}

private:
    void (*cookie_)();
    char buf_[SIZE];
    char* cur_;
};


class LogStream {
public:
    typedef FixedBuffer<kSmallSize> Buffer;
    typedef LogStream Self;

    LogStream() {}
    ~LogStream() {}

    Buffer& buffer() {return buffer_;}

    template <typename T>
    void FormatInteger(T num) {
        if (buffer_.writablebytes() >= kNum2MaxStringLength) {
            char* buf = buffer_.current();
            char* now = buf;
            const char* zero = digits + 9;
            bool negitive = num < 0;

            do {
                int remainder = static_cast<int>(num % 10);
                *(now++) = zero[remainder];
                num /= 10;
            } while (num != 0);

            if (negitive) *(now++) = '-';
            *now = '\0';
            std::reverse(buf, now);
            buffer_.Add(static_cast<int>(now - buf));
        }
    }

    Self& operator<<(short num) {
        return (*this) << static_cast<int>(num);
    }

    Self& operator<<(unsigned short num) {
        return (*this) << static_cast<unsigned int>(num);
    }

    Self& operator<<(int num) {
        FormatInteger(num);
        return *this;
    }

    Self& operator<<(unsigned int num) {
        FormatInteger(num);
        return *this;
    }

    Self& operator<<(long num) {
        FormatInteger(num);
        return *this;
    }

    Self& operator<<(unsigned long num) {
        FormatInteger(num);
        return *this;
    }

    Self& operator<<(long long num) {
        FormatInteger(num);
        return *this;
    }

    Self& operator<<(unsigned long long num) {
        FormatInteger(num);
        return *this;
    }

    Self& operator<<(const float& num) {
        return (*this) << static_cast<double>(num);
    }

    Self& operator<<(const double& num) {
        char buf[32];
        int len = snprintf(buf, sizeof(buf), "%g", num);
        buffer_.Append(buf, len);
        return *this;
    }

    Self& operator<<(bool boolean) {
        return (*this) << (boolean ? 1 :0);
    }

    Self& operator<<(char chr) {
        buffer_.Append(&chr, 1);
        return *this;
    }

    Self& operator<<(const void* data) {
        return (*this) << static_cast<const char*>(data);
    }

    Self& operator<<(const char* data) {
        buffer_.Append(data, static_cast<int>(strlen(data)));
        return *this;
    }

    Self& operator<<(const GeneralTemplate& source) {
        buffer_.Append(source.data_, source.len_);
        return *this;
    }

    Self& operator<<(const std::string& str) {
        buffer_.Append(str.data(), static_cast<int>(str.size()));
        return *this;
    }

private:
    Buffer buffer_;
};

}

#endif