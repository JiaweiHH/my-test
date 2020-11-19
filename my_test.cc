#include <unistd.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/resource.h>

static const int pagesize = 1024 * 4;
static const int pagenum = 500;

//30个进程单核. CFS 5m36s, 自定义4m1s
//30个进程多核. CFS 1m26s, 自定义41s

void Run(int idx)
{
    if(idx % 2) {
        int a,b;
        for(int j = 0;j < 50;j++)
        {
            for(int i = 0; i < 120000000; i++) 
            {
                a = b;
            }    
        }
    } else {
        int a,b;
        for(int j = 0;j < 50;j++)
        {
            for(int i = 0; i < 120000000; i++) 
            {
                a = b;
            }    
            if(j % 10 == 0) {
                char* p = (char*)malloc(pagesize * pagenum);
                memset(p,1,pagesize * pagenum);
            }
        }
    }
}

 /* 绑定CPU亲和性，单核调用 */
void set_affinity(){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(2, &mask);
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);
}

/* 
 * 设置进程的nice值，内核会转化为对应的static_prio 
 * 当前设置为1，内核对应的static_prio为121
 */
void set_priority(){
    pid_t pid = getpid();
    int oldpri = getpriority(PRIO_PROCESS, pid);
    setpriority(PRIO_PROCESS, getpid(), 1);
    int newpri = getpriority(PRIO_PROCESS, pid);
    printf("oldpri: %d, newpri: %d\n", oldpri, newpri);
}

int main(int argc, char const *argv[])
{
    set_affinity();
    set_priority();

    for(int i = 0;i < 10;i++)
    {
        if(fork() == 0) {
            Run(i);
            break;
        } else {

        }
    }
    int status = 0;
    int corpse;

    while ((corpse = waitpid(0, &status, 0)) > 0)
    return 0;
}