//
// Created by ideal2 on 19-6-26.
//

#include "discard.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(2009);
    DiscardServer server(&loop, listenAddr);
    server.start();
    loop.loop();

    return 0;
}