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

void compare2Target(char* path, char* target){
    char* p;
    for(p=path+strlen(path); p>=path && *p != '/'; p--);
    p++;
    if(target == NULL){
        fprintf(1, "%s\n", path);
        return;
    }
    if(!strcmp(p, target)){
        fprintf(1, "%s\n", path);
    }
}

void addSlash(char* path){
    if(path[strlen(path)-1] != '/'){
        path[strlen(path)] = '/';
        path[strlen(path)] = 0;
    }
}

void find(char* path, char* target){
    int fd;
    struct stat st;
    // 如果打开文件失败，直接返回
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "cannot open %s\n", path);
        return;
    }
    // 如果获取文件信息失败，直接返回
    if(fstat(fd, &st) < 0){
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        // 如果path是文件或者设备，判断和target是否相等
        case T_DEVICE:
        case T_FILE:
            // 判断和target是否相等，如果相等输出path
            compare2Target(path, target);
            break;
        // 如果是文件夹，找到文件夹中所有的项，对每一项递归find（除了.和..）
        case T_DIR:
            // 首先给路径加上'/'
            addSlash(path);
            struct dirent de;
            char * p;
            p = path + strlen(path);
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                // 跳过对.和..的递归
                if(!strcmp(".", de.name) || !strcmp("..", de.name)){
                    continue;
                }
                // 现在是path = path/de.name
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '0';
                // 继续递归
                find(path, target);
            }
            break;
    }
    close(fd);
}

int main(int argc, char* argv[]){
    char path[512]={'0'};
    char* target = NULL;
    if(argc == 1){
        memmove(path, ".", strlen("."));
        target = NULL;
    } else if(argc == 2){
        memmove(path, argv[1], strlen(argv[1]));
        target = NULL;
    }else if(argc == 3){
        memmove(path, argv[1], strlen(argv[1]));
        target = argv[2];
    }else{
        fprintf(2, "error: too many arguments\n");
    }
    find(path, target);
    exit(0);
}