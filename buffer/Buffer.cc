#include "Buffer.h"
#include <sys/uio.h>
#include <cerrno>

//从fd中读取数据放到buffer中
//如果数据不多 直接读到buffer,如果超过writable，就把超出的先读到extrabuf
//再append到buffer
ssize_t Buffer::readFd(int fd, int *savedErrno) {

    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    //如果可写的区间长度小于extrabuf，则两个都传入，否则只用一个
    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = readv(fd, vec, iovcnt);
    if(n < 0)
    {
        *savedErrno = errno;
    }
    else if(static_cast<size_t >(n) <= writable)
    {
        writerIndex_ += n;//减小空闲区
    }
    else
    {
        writerIndex_ = buffer_.size();//否则把空闲区清零
        append(extrabuf, n - writable);//如果buffer不够就会发生vector的扩充
    }

    return n;

}