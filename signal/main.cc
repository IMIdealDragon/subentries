/*
 * @Copyright(C): Ideal Dragon. All rights reserved. 
 * @lisence: GPL
 * @Author: Ideal Dragon
 * @Date: 2019-07-12 17:10:48
 * @Description: 
 */
/*
 *
           struct sigaction {
               void     (*sa_handler)(int);   //表示新的处理函数
               void     (*sa_sigaction)(int, siginfo_t *, void *);//这一项和sa_handler只能有一个
               sigset_t   sa_mask;   //屏蔽信号集
               int        sa_flags;  //用来处理一些内核给定的可以选择的信号处理动作
               void     (*sa_restorer)(void);//应用程序不用管
           };

 * */
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <functional>
#include "flyd_signal.h"
//#include <bits/sigaction.h>

void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext);
//一个信号有关的结构 ngx_signal_t
typedef struct
{
    int           signo;       //信号对应的数字编号 ，每个信号都有对应的#define ，大家已经学过了
    const  char   *signame;    //信号对应的中文名字 ，比如SIGHUP

    //信号处理函数,这个函数由我们自己来提供，但是它的参数和返回值是固定的【操作系统就这样要求】,大家写的时候就先这么写，也不用思考这么多；
    void  (*handler)(int signo, siginfo_t *siginfo, void *ucontext); //函数指针,   siginfo_t:系统定义的结构
} ngx_signal_t;

ngx_signal_t  signals[] = {
    // signo      signame             handler
//    { SIGHUP,    "SIGHUP",           ngx_signal_handler },        //终端断开信号，对于守护进程常用于reload重载配置文件通知--标识1
//    { SIGINT,    "SIGINT",           ngx_signal_handler },        //标识2
//	{ SIGTERM,   "SIGTERM",          ngx_signal_handler },        //标识15
//    { SIGCHLD,   "SIGCHLD",          ngx_signal_handler },        //子进程退出时，父进程会收到这个信号--标识17
//    { SIGQUIT,   "SIGQUIT",          ngx_signal_handler },        //标识3
//    { SIGIO,     "SIGIO",            ngx_signal_handler },        //指示一个异步I/O事件【通用异步I/O信号】
//    { SIGSYS,    "SIGSYS, SIG_IGN",  NULL               },        //我们想忽略这个信号，SIGSYS表示收到了一个无效系统调用，如果我们不忽略，进程会被操作系统杀死，--标识31
//                                                                  //所以我们把handler设置为NULL，代表 我要求忽略这个信号，请求操作系统不要执行缺省的该信号处理动作（杀掉我）
//    //...日后根据需要再继续增加
//    { 0,         NULL,               NULL               }         //信号对应的数字至少是1，所以可以用0作为一个特殊标记
};


void signal_handle(int sig_num)
{
    if(sig_num == SIGUSR1)
    {
        printf("Capture SIGUSR1\n");
    }
    printf("signal_handle running ...\n");
}

//参数1：信号   参数2：信号的各种信息  参数3：当前进程的上下文
 void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext)
{
    //printf("来信号了\n");

    printf("USR2信号来了\n");
    printf("signo == %d\n", signo);
    printf("signo == %d\n", siginfo->si_uid);

}

typedef std::function<void(int,siginfo_t*, void*)> SigFunction;
int main(int argc,char **argv)
{
    flyd::Signal& sig = flyd::Signal::GetInstance();
    sig.Init();
 //   signal(SIGUSR1,signal_handle);

    struct sigaction sa;// 定义一个管理信号的结构体
    sa.sa_sigaction = ngx_signal_handler;
    sa.sa_flags = SA_SIGINFO;//令sa_sigaction指定的信号处理程序生效

    sigemptyset(&sa.sa_mask);


   if( sigaction(SIGUSR2, &sa, NULL) < 0)//参数1：要操作的信号，参数2：处理函数，参数3：以往处理函数
    printf("安装失败！\n");


    while(1)
    {
        printf("pid = %d", getpid());
        printf("等待信号\n");
        sleep(2);
    }


    return 0 ;
}