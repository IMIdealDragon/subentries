cmake_minimum_required(VERSION 2.6)

project(signal C CXX)

add_compile_options(-std=c++11)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(signal main.cc flyd_signal.cc)


if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  MESSAGE(STATUS "current platform: Windows")
  include_directories("F:\\usr\\local\\include\\boost-1_70")
  include_directories("D:\\linux_shared\\include")
  include_directories("D:\\linux_shared\\include\\x86_64-linux-gnu")
  include_directories("D:\\linux_shared\\include\\x86_64-linux-gnu\\sys")
  include_directories("D:\\linux_shared\\include\\x86_64-linux-gnu\\bits")
endif ()

#export()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 )








