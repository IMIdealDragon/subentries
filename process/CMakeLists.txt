cmake_minimum_required(VERSION 2.6)
project(process CXX C)

add_definitions("-Wall -g -std=c++11")
set(CMAKE_CXX_STANDARD 11)


add_executable(flyd_process main.cpp flyd_process.cc)