//
// Created by xmh on 24-3-6.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#ifndef NULL
#define NULL 0
#endif
int main(){
    // 用来存储从标准输入中读取到的字符
    char line[512] = {0};
    // 存储exec用来执行的命令
    char* cmds[4] = {"grep", "hello"};
    char* p = line;
    int pid;
    // 从标准输入中不断读取字符
    for(; read(0, p, 1); p++){
        // 读不到'\n'，继续读
        if(*p != '\n'){
            continue;
        }else{
            // 读到'\n'，已经得到一行
            *p = 0;
            pid = fork();
            if(pid == 0){
                // 子进程中执行命令
                cmds[2] = line;
                cmds[3] = NULL;
                exec("grep", cmds);
            }else{
                wait(0);
                // 将p重新指向line数组开头，继续读下一行
                p = line;
            }
        }
    }
    exit(0);
}