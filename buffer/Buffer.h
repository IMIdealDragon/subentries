//
// Created by ideal2 on 19-6-21.
//

#ifndef BUFFER_BUFFER_H
#define BUFFER_BUFFER_H

#include <cstdio>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>

/*+---------------+------------------+-------------------+
 *|prependable    |  readable bytes  | writable bytes    |
 *+---------------+------------------+-------------------+
 *|  |            |                  |                   |
 *0 kcheap <= readerIndex    <=    writerIndex   <=     size
 */

//0～readerIndex是放数据的区间，writerIndex～size是空闲的可写区间
//append把数据加入到writable后面
//retrieve取回，从buffer中取出数据


class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize),
          readerIndex_(kCheapPrepend),
          writerIndex_(kCheapPrepend)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initialSize);
        assert(prependableBytes() == kCheapPrepend);
    }

    //定义一些接口函数，只是读取数据的，都是不可改变的，用const
    size_t readableBytes() const //返回可读字节数
    {return writerIndex_ - readerIndex_;}

    size_t writableBytes() const //返回可读字节数
    {return buffer_.size() - writerIndex_;}

    size_t prependableBytes() const  //const函数必须调用const
    {return readerIndex_;}

    const char* peek() const //返回读指针的地址
    { return begin() + readerIndex_; }

    void retrieve(size_t len)//读取buffer内容以后的指针移动
    {
        assert(len <= readableBytes());
        if(len < readableBytes())
        {
            readerIndex_ += len;
        }
        else
        {
            retrieveAll();
        }
    }

    void retrieveAll()//数据全部取走的指针移动
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    //从buffer中取出len字节数据
    std::string retrieveAsString(size_t len)
    {
        assert(len <= readableBytes());
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }

    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }


    //向buffer中读数据
    ssize_t readFd(int fd, int* savedErrno);

    //向buffer中写数据
    void append(const char* data, size_t len)
    {
        ensureWritableBytes(len);//确保当前的buffer能容下这么多数据
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    //向buffer中写数据 string参数
    void append(const std::string& s)
    {
        append(s.data(), s.size());
    }


    //返回写地址
    char* beginWrite()
    {return begin() + writerIndex_;}

    const char* beginWrite() const
    {return begin() + writerIndex_;}

    //更新写指针，表示已经写入len
    void hasWritten(size_t len)
    {
        assert(len <= writableBytes());
        writerIndex_ += len;
    }

    //确保写入区间空间的函数：
    void ensureWritableBytes(const size_t len)
    {
        if(writableBytes() < len)//可写区间长度小于要写入的区间长度
        {
            makeSpace(len);//扩充区间
        }
        assert(writableBytes() >= len);
    }

private:
    //如果内存不够，则扩充内存
    void makeSpace(size_t len)//扩充buffer的空间
    {
        if(writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writerIndex_ + len);//vector扩充大小，可以了解一下动态过程
        }

        else//如果prependable的区间拿过来够用
        {
            assert(kCheapPrepend < readerIndex_);
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                    begin() + writerIndex_,
                    begin() + kCheapPrepend);//把readable区间前移
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_;
            assert(readable == readableBytes());//readable区间不变
        }
    }




private:

    char* begin()
    {return &*buffer_.begin();}//begin返回迭代器，取得值再取地址

    const char* begin() const  //const重载为了让const对象也可以调用
    {return &*buffer_.begin();}

    std::vector<char > buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};

#endif //BUFFER_BUFFER_H
