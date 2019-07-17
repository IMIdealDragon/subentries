#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include "flyd_process.h"

pid_t flyd_pid, flyd_parent;

int main() {

    flyd_pid = getpid();      //取得进程pid
    flyd_parent = getppid();     //取得父进程的id

    printf("主进程pid = %d, ppid = %d\n", flyd_pid, flyd_parent);

    char* par_name = "flyd_par";

    prctl(PR_SET_NAME, par_name);

    int cdaemonresult = flyd_daemon();
    if(cdaemonresult == -1) //fork()失败
        printf("cdaemonresult:master进程创建失败\n");
    if(cdaemonresult == 1)
    {
            printf("这是原始的父进程，退出\n");
            exit(0);
    }

    flyd_master_process_cycle();
//    while(1);

    //工作进程开始循环

    return 0; //子进程返回0
}