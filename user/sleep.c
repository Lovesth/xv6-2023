//
// Created by xmh on 24-3-5.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    // 只有一个参数，说明sleep确实时间参数，错误
    if(argc == 1){
        fprintf(2, "sleep: need a specified integer\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    // sleep时间小于0，错误
    if(n < 0){
        fprintf(2, "the time couldn't be negative\n");
        exit(1);
    }
    // sleep
    sleep(n);
    fprintf(1, "sleep finished\n");
    exit(0);
}