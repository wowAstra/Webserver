#include "tcpconnection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>

#include "channel.h"
#include "buffer.h"

using namespace my_muduo;

TcpConnection::TcpConnection(EventLoop* loop, int connfd)
    :loop_(loop), fd_(connfd), shutdown_(false), channel_(new Channel(loop_, fd_)) {
    channel_->SetReadCallback(std::bind(&TcpConnection::HandleMessage, this));
    channel_->SetWriteCallback(std::bind(&TcpConnection::HandleWrite, this));
}

void TcpConnection::HandleMessage() {
    int read_size = input_buffer_.ReadFd(fd_);
    if (read_size > 0) {
        message_callback_(this, &input_buffer_);
    }
}

void TcpConnection::HandleWrite() {
    int len = output_buffer_.readablebytes();
    int remaining = len;
    int send_size = send(fd_, output_buffer_.Peek(), remaining, 0);
    remaining -= send_size;

    assert(remaining <= len);
    if (!remaining) {
        channel_->DisableWriting();
    }
}

void TcpConnection::Send(const char* message, int len) {
    int remaining = len;
    int send_size = 0;
    if (!channel_->IsWriting() && output_buffer_.readablebytes() == 0) {
        send_size = send(fd_, message, len, 0);
        remaining -= send_size;
    }

    assert(remaining <= len);
    if (remaining > 0) {
        output_buffer_.Append(message + send_size, remaining);
        if (!channel_->IsWriting()) {
            channel_->EnableWriting();
        }
    }
}

void TcpConnection::Send(Buffer* buffer) {
    Send(buffer->Peek(), buffer->readablebytes());
}

void TcpConnection::Send(const string& message) {
    Send(message.data(), sizeof(message.size()));
}

