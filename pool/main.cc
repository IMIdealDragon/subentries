#include <iostream>
#include <string>
#include <future>   //std::future
#include <thread>   //std::this_thread::sleep_for
#include <chrono>   //std::chrono::seconds

#include "threadpool.h"
int main()
{
    //创建一个能够并发执行四个线程的线程池
    ThreadPool pool(4);
    pool.start();

    //创建并发执行线程的结果列表
    std::vector< std::future<std::string> > results;

    //启动八个需要执行的线程任务
    for(int i = 0; i < 8; ++i)
    {
        //将并发执行任务的返回值添加到结果列表中
        results.emplace_back(
            //将下面的打印任务添加到线程池中并发执行
            pool.enqueue([i]{
                std::cout << "hello " << i << std::endl;
                //上一行输出后，让线程等待一秒钟
                std::this_thread::sleep_for(std::chrono::seconds(1));
                //然后再继续输出并返回执行结果
                std::cout << "world " << i << std::endl;

                return std::string("---thread ") + std::to_string(i) + std::string(" finished----\n");
            })
        );
    }

   //输出线程任务执行的结果
    for(auto && result:results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    return 0;
}