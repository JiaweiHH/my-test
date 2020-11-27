#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <linux/futex.h>
#include <sys/syscall.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(){
    key_t key_n=ftok("./",'s');
    printf("key num: %d\n", key_n);

    int shm_id = shmget(key_n, 1024, 0666|IPC_CREAT);
    printf("share memory id: %d\n",shm_id);

    pid_t pid = fork();
    if(pid = fork())
    return 0;
}