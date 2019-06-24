//
// Created by ideal2 on 19-6-21.
//
#include <iostream>
#include "Buffer.h"
#include <string>
#include "UnitTest.h"
using namespace std;
void test1()
{
    printf("test1-------------------------------\n");
    Buffer test;
    const std::string str(200, 'x');

    test.append(str);
    printf("readableBytes %d == %d size\n",
           test.readableBytes(),str.size());
    printf("writableBytes %d == %d size\n",
           test.writableBytes(),Buffer::kInitialSize - str.size());
    printf("prependableBytes %d == %d kCheapPrepend\n",
           test.prependableBytes(),Buffer::kCheapPrepend);

    const string str2 = test.retrieveAsString(50);

    printf("readableBytes %d == %d size\n",
           str2.size(),50);
    printf("readableBytes %d == %d size\n",
           test.readableBytes(),str.size() - str2.size());
    printf("writableBytes %d == %d size\n",
           test.writableBytes(),Buffer::kInitialSize - str.size());
    printf("prependableBytes %d == %d kCheapPrepend\n",
           test.prependableBytes(),Buffer::kCheapPrepend + str2.size());


    test.append(str);
    printf("readableBytes %d == %d size\n",
           test.readableBytes(),2*str.size() - str2.size());
    printf("writableBytes %d == %d size\n",
           test.writableBytes(),Buffer::kInitialSize - 2*str.size());
    printf("prependableBytes %d == %d kCheapPrepend\n",
           test.prependableBytes(),Buffer::kCheapPrepend + str2.size());

    const string str3 = test.retrieveAllAsString();
    std::cout << str3 << std::endl;
}

