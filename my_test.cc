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

static const int pagesize = 1024 * 4;
static const int pagenum = 500;
static const int map_size = 4096 * 4;
static const int file_size = 390798947;
static const int loop = 95410;
char file_name[] = {"logtest.log"};

//建立内存映射
void MapRegion(int fd, uint64_t file_offset, char **base){
    void *ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, file_offset);
    if(ptr == MAP_FAILED)
        *base = NULL;
    else
        *base = (char *)ptr;
}

//取消内存映射
void UnMapRegion(char *base){
    munmap(base, map_size);
}

void run_cpu(int idx)
{
    int a,b;
    for(int j = 0;j < 50;j++)
    {
        for(int i = 0; i < 120000000; i++) 
        {
            a = b;
        }    
    }
}

void run_cpu_mem(int idx)
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
                char* p = (char*)malloc(pagesize * idx * 100);
                memset(p,1,pagesize * idx * 100);
            }
        }
    }
}

//kernel->user->file, 磁盘读取次数少
void run_io_buffer(int idx){
    char buf_output[20], buf[BUFSIZ];
    sprintf(buf_output, "output-%d", idx);

    FILE *file = fopen(file_name, "r");
    FILE *output_file = fopen(buf_output, "w");

    while(fgets(buf, BUFSIZ, file) != NULL){
        fputs(buf, output_file);
    }
    
    fclose(output_file);
    fclose(file);
}

//user->file
void run_io_direct(int idx){
    char buf_output[20];
    sprintf(buf_output, "output-%d", idx);
    unsigned char *buf;
    int ret = posix_memalign((void **)&buf, getpagesize(), BUFSIZ); //对齐。保证缓冲区起始位置以及每次读写都是逻辑块的整数倍
    if(ret){
        printf("posix_memalign failed!!\n");
        return;
    }

    int fd_r = open(file_name, O_RDONLY|O_DIRECT); //直接IO形式打开文件
    int fd_w = open(buf_output, O_WRONLY|O_DIRECT|O_CREAT); //直接IO形式打开文件，如果不存在就创建该文件
    if(fd_r < 0 || fd_w < 0){
        printf("open file failed!!\n");
        return;
    }

    int ret_w, ret_r;
    do{
        ret_r = read(fd_r, buf, BUFSIZ);
        ret_w = write(fd_w, buf, BUFSIZ);
        // printf("---%d---%d---%d\n", idx, ret_r, ret_w);
    }while(ret_r > 0 && ret_w > 0);
}

void run_io_mmap(int idx){
    char buf_output[20];
    sprintf(buf_output, "output-%d", idx);
    
    int fd_r = open(file_name, O_RDWR|O_DIRECT);
    char *base_r = NULL, *cursor_r = NULL; //读取文件的内存映射指针
    posix_fallocate(fd_r, 0, file_size);
    MapRegion(fd_r, 0, &base_r);
    if(base_r == NULL){
        printf("read mapregion error!!\n");
        return;
    }

    int fd_w = open(buf_output, O_RDWR|O_CREAT|O_DIRECT);
    char *base_w = NULL, *cursor_w = NULL; //写入文件的内存映射指针
    posix_fallocate(fd_w, 0, file_size);
    MapRegion(fd_w, 0, &base_w);
    if(base_w == NULL){
        printf("wirte mapregion error!!\n");
        return;
    }

    cursor_r = base_r;
    cursor_w = base_w;
    uint64_t file_offset = map_size, mmap_offset = 0;

    /*
     * 从fd_r中每次读取pagesize的数据，拷贝到fd_w
     * 每16K重置一次
     */
    for(int i = 0; i < loop; ++i){
        if(mmap_offset >= map_size){
            UnMapRegion(base_r);
            UnMapRegion(base_w);
            MapRegion(fd_r, file_offset, &base_r);
            MapRegion(fd_w, file_offset, &base_w);
            if(base_r == NULL || base_w == NULL){
                printf("error!!\n");
                return;
            }
            cursor_r = base_r;
            cursor_w = base_w;
            file_offset += map_size;
            mmap_offset = 0;
        }
        memcpy(cursor_w, cursor_r, pagesize);
        cursor_r += pagesize;
        cursor_w += pagesize;
        mmap_offset += pagesize;
    }

    UnMapRegion(base_r);
    UnMapRegion(base_w);
    close(fd_r);
    close(fd_w);
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
}

int main(int argc, char const *argv[])
{
    set_affinity();
    //set_priority();

    for(int i = 0;i < 10;i++)
    {
        if(fork() == 0) {
            run_cpu_mem(i);
            break;
        } else {

        }
    }
    int status = 0;
    int corpse;

    while ((corpse = waitpid(0, &status, 0)) > 0)
    return 0;
}
