cmake_minimum_required(VERSION 3.14)
project(buffer)

set(CMAKE_CXX_STANDARD 11)
set(BUFFER_SRC
        main.cc
        Buffer.cc
        UnitTest.cc UnitTest.h)

add_executable(buffer ${BUFFER_SRC})