#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// stdin -> command args
// exec的argv最后一个参数必须为0，否则将执行失败
int
main(int argc, char *argv[])
{
    char stdinBuff[1024] = {0};
    int offset = 0;
    //printf("stdin:%s", stdinBuff);

    // printf("argc = %d\n", argc);
    // printf("arg0 = %s\n", argv[0]); 
    // printf("arg1 = %s\n", argv[1]);
    // printf("arg2 = %s\n", argv[2]);

    for(;;){
        offset = 0;
        while(1){
            //0stdin 1stdout 2stderr
            int n = read(0, stdinBuff+offset, 1);

            if(n == 0){
                stdinBuff[offset] = '\0';
                break; 
            }else if(stdinBuff[offset] == '\n'){
                stdinBuff[offset] = '\0';
                break;
            }else if(stdinBuff[offset] == ' '){
                stdinBuff[offset] = '\0';
                break;
            }
            
            offset++;  
        }
        if(offset == 0){
            break;
        }
        //printf("p:%s", stdinBuff);
        // argv1 xargs要执行的程序
        // argv2 xargs要执行的程序的参数，这里准备一个小的参数数组，假定就一个参数，也能通过测试
        char *argvs[4] ={0};
        argvs[0] = argv[1];
        argvs[1] = argv[2];
        argvs[2] = stdinBuff;
        argvs[3] = 0;

        if(fork() == 0){
            //子进程执行exec
            //exec的第二个参数，还是要额外带一下这个程序的名字，即使第一个参数已经有了
            // printf("argv[1]=%s ", argv[1]);
            // printf("argvs[0]=%s ", argvs[0]);
            // printf("argvs[1]=%s ", argvs[1]);
            // printf("argvs[2]=%s\n", argvs[2]);

            exec(argv[1], argvs);
            exit(0);
        }else{
            wait(0);
        }
    }
    exit(0);
}