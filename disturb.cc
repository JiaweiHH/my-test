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
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>

int main(){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(2, &mask);
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);
    
     char *disturb;
     disturb = (char *)malloc(4096*10);
     memset(disturb, 0, 4096*10);
    while(1);
    return 0;
}
