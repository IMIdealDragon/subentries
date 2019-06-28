//
// Created by ideal2 on 19-6-26.
//

#ifndef MUDUO_MASTER_DISCARD_H
#define MUDUO_MASTER_DISCARD_H

#include "muduo/net/TcpServer.h"

class DiscardServer {
public:
    DiscardServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr);
    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buf,
            muduo::Timestamp time);

    muduo::net::TcpServer server_;
};


#endif //MUDUO_MASTER_DISCARD_H
