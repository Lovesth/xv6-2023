//
// Created by xmh on 24-3-5.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, int* argv[]){
    int fd[2];
    // 生成管道，用于在父子进程之间传递数据
    pipe(fd);
    // 用于存放在管道中传递的字节
    char data[1];
    int pid = fork();
    if(pid < 0){
        fprintf(2, "fork error\n");
        exit(-1);
    }else if(pid > 0){
        // 父进程，写一个字节到管道
        write(fd[1], "b", 1);
        read(fd[0], data, 1);
        close(fd[1]);
        pid = getpid();
        fprintf(1, "%d: received pong\n", pid);
        close(fd[0]);
        wait(0);
    }else{
        // 子进程
        read(fd[0], data, 1);
        pid = getpid();
        fprintf(1, "%d: received ping\n", pid);
        close(fd[0]);
        write(fd[1], data, 1);
        close(fd[1]);
    }
    exit(0);
}
