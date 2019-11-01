/*
 * @Author: Ideal Dragon
 * @Date: 2019-10-31 20:09:57
 * @email: zhilong_jiang@126.com
 * @Description: threadpool.h
 */
#pragma once
#include <future>
#include <queue>

class ThreadPool {

public:
typedef std::function<void()> Task;
    //在线程池中创建threads个工作线程
    ThreadPool(size_t threads);

    void start();
    Task take();

    //向线程池中增加线程  可变参数模板
template<typename F, typename... Args>
auto enqueue(F&& f, Args&&... args)  //尾置返回类型
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    //推导任务返回类型，result_of推导F(Args...)的返回类型
    using return_type = typename std::result_of<F(Args...)>::type;

    //获得当前任务，将一个返回值为return_type,参数为args的任务函数f封装，
    //并得到它的智能指针task
    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    //获得 std::future 对象 以供实施线程同步
    std::future<return_type> res = task->get_future();
   
   //临界区
   {
       std::unique_lock<std::mutex> lock(queue_mutex);

       //禁止在线程池停止后加入新的线程池
       if(!running_)
        throw std::runtime_error("enqueue on stopped ThreadPool");

        //将线程池添加到执行任务队列中,这个用emplace_back应该也是一样的吧
        tasks.emplace( [task]{ (*task)(); } );
   }


//通知一个正在等待的线程
    condition.notify_one();
    return res;
}
    
    ~ThreadPool();

private:
    //需要持续追踪线程来保证可以使用join
    std::vector< std::thread > workers;//线程向量
    //任务队列
    std::queue< std::function<void()> > tasks;

    //同步相关原语
    std::mutex queue_mutex;    //互斥锁
    std::condition_variable condition; //互斥条件变量

    //线程池运行标志
    bool running_;
    int  numThreads_;
};