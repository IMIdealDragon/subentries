cmake_minimum_required(VERSION 2.6)
project(threadpool)
add_compile_options(-std=c++11 -g)

set(CMAKE_CXX_STANDARD 11)
set(BUFFER_SRC
        main.cc
        threadpool.cc
        )

add_executable(threadpool ${BUFFER_SRC})
target_link_libraries(threadpool pthread)
