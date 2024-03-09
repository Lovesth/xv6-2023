////
//// Created by xmh on 24-3-5.
////
//#include "kernel/types.h"
//#include "kernel/stat.h"
//#include "kernel/fcntl.h"
//#include "user/user.h"
//
//int main() {
//    int fd_c[2];
//    int fd_n[2];
//    pipe(fd_c);
//    pipe(fd_n);
//    int num;
//    // 管道从左边写，从右边读。
//    // 对于相邻的两个管道p1->p2，从p1右边读，写到p2左边
//    for(int i=2; i<35; i++){
//        write(fd_c[1], &i, sizeof(int));
//    }
//    for (int i = 2; i < 35; i++) {
//        // 这里一定要关闭管道写端，若不关闭，管道中没有数据时，read阻塞。
//        close(fd_c[1]);
//        if (read(fd_c[0], &num, sizeof(int))){
//            fprintf(1, "prime %d\n", num);
//        }else{
//            break;
//        }
//        while (read(fd_c[0], &num, sizeof(int))){
//            if (num % i) {
//                write(fd_n[1], &num, sizeof(int));
//            }
//        }
//        close(fd_c[0]);
//        fd_c[0] = fd_n[0];
//        fd_c[1] = fd_n[1];
//        pipe(fd_n);
//    }
//    exit(0);
//}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(){
    int fd_p[2];
    int fd_c[2];
    pipe(fd_p);
    int pid;
    int num;
    int p;

    pid = fork();
    if(pid > 0){
        for(int i=2; i<=35; i++){
            write(fd_p[1], &i, sizeof(int));
        }
        close(fd_p[0]);
        close(fd_p[1]);
    }else if(pid == 0){
        while (1){
            close(fd_p[1]);
            if(read(fd_p[0], &p, sizeof(int))){
                fprintf(1, "prime %d\n", p);
            } else{
                break;
            }
            pid = fork();
            if(pid > 0){
                close(fd_p[0]);
                break;
            }else{
                pipe(fd_c);
                while(read(fd_p[0], &num, sizeof(int))){
                    if(num % p)
                        write(fd_c[1], &num, sizeof(int));
                }
                close(fd_p[0]);
                fd_p[0] = fd_c[0];
                fd_p[1] = fd_c[1];
            }
        }
    }else{
        fprintf(2, "fork error\n");
        exit(-1);
    }
    wait(0);
    exit(0);
}