/*
 * @Author: Ideal Dragon
 * @Date: 2019-10-31 21:09:06
 * @email: zhilong_jiang@126.com
 * @Description: 
 */
#include "threadpool.h"
#include <cassert>
#include <iostream>


ThreadPool::ThreadPool(size_t threads)
:numThreads_(threads), running_(false)
{

}

typedef std::function<void()> Task;
//从任务队列中取出一个任务放到线程
Task ThreadPool::take()
{
    //创建互斥锁
    std::unique_lock<std::mutex> lock(queue_mutex);

    //注释内容和下面这行等价,任务队列为空且运行状态则执行等待
    //如果当前的队列不为空或者是停止等待状态，那我就不用再执行wait，我就去执行任务或者退出了
    // while(tasks.empty() && running_)
    // {
    //     condition.wait(lock);
    // }
    condition.wait(lock,[this]{return tasks.empty() && running_;});

   //如果当前线程池已经结束且等待队列为空，则应该直接返回
    if(!running_ && tasks.empty())
        return nullptr;
    std::function<void()> task;
    if(!tasks.empty())
    {
        task = std::move(tasks.front());
        tasks.pop();
    }

    return task;
}




//启动函数只负责启动一定数量的工作线程
void ThreadPool::start()
{
    assert(workers.empty());
    running_ = true;
    workers.reserve(numThreads_);
    //启动 threads 数量的工作线程
    for(size_t i = 0; i < numThreads_; ++i)
    {
        workers.emplace_back([this]{
            while(running_)
            {
                Task task(take());
                if(task)
                    task();
            }
        });
    }

}


//销毁所有线程池中创建的线程
ThreadPool::~ThreadPool()
{
    //临界区
    {
        //创建互斥锁
        std::unique_lock<std::mutex> lock(queue_mutex);

        //停止线程池
        running_ = false;
    }

    //通知所有等待线程
    condition.notify_all();

    //使所有的异步线程转为同步执行,等待工作线程执行结束
    for(std::thread &worker:workers)
        worker.join();
}


//向线程池中添加新任务
//1.支持多个入队参数需要使用变长模板参数
//2.为了调度执行的任务，需要包装执行的任务，这就意味着需要对任务函数的类型进行包装、构造
//3.临界区可以在一个作用域里面被创建，最佳实践是RAII的形式
// template<typename F, typename... Args>
// auto ThreadPool::enqueue(F&& f, Args&&... args)  //尾置返回类型
//     -> std::future<typename std::result_of<F(Args...)>::type>
// {
//     //推导任务返回类型，result_of推导F(Args...)的返回类型
//     using return_type = typename std::result_of<F(Args...)>::type;

//     //获得当前任务，将一个返回值为return_type,参数为args的任务函数f封装，
//     //并得到它的智能指针task
//     auto task = std::make_shared< std::packaged_task<return_type()> >(
//         std::bind(std::forward<F>(f), std::forward<Args>(args)...)
//     );

//     //获得 std::future 对象 以供实施线程同步
//     std::future<return_type> res = task->get_future();
   
//    //临界区
//    {
//        std::unique_lock<std::mutex> lock(queue_mutex);

//        //禁止在线程池停止后加入新的线程池
//        if(stop)
//         throw std::runtime_error("enqueue on stopped ThreadPool");

//         //将线程池添加到执行任务队列中,这个用emplace_back应该也是一样的吧
//         tasks.emplace( [task]{ (*task)(); } );
//    }


// //通知一个正在等待的线程
//     condition.notify_one();
//     return res;
// }