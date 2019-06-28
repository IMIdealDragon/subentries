//
// Created by ideal2 on 19-6-26.
//

#include "discard.h"
#include "muduo/base/Logging.h"

using namespace muduo;
using namespace muduo::net;

DiscardServer::DiscardServer(muduo::net::EventLoop *loop,
        const muduo::net::InetAddress &listenAddr)
        : server_(loop, listenAddr, "DiscardServer"){
    server_.setConnectionCallback(
            std::bind(&DiscardServer::onConnection, this, _1));
    server_.setMessageCallback(
            std::bind(&DiscardServer::onMessage, this, _1, _2, _3));
}


void DiscardServer::start() {
    server_.start();
}

void DiscardServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,
                              muduo::Timestamp time) {
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << "message is " << msg.c_str();
    LOG_INFO << conn->name() << "discards " << msg.size()
             << " byte received at " << time.toString();
}

void DiscardServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {
    LOG_INFO << "DiscardServer - " << conn->peerAddress().toIpPort()
            << " -> " << " is " << (conn->connected() ? "UP" : "DOWN");
}