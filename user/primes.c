#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
If no data is available, 
a read on a pipe waits for either data to be written 
or for all file descriptors referring to the write end to be closed

作者：炼金术士
链接：https://zhuanlan.zhihu.com/p/346495822
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

说是Hint里面有这句话，但是我没找到。
这句话很关键，就是必须父子进程都关闭了一个管道的写端，那么读端才能读到0.
在这里卡了很久。
*/
int
main(int argc, char *argv[])
{
    int nPPID = -1;
    int nPID = getpid();

    int aPipeReadFromParent[2];
    int aPipeWriteToChild[2];

    pipe(aPipeReadFromParent);
    pipe(aPipeWriteToChild);

    int nFirstRead = -1;
    int bForked = 0;

    for(int i=2; i<=35; i++){
        int t = i;
        write(aPipeReadFromParent[1], &t, 4);
    }

    //close(aPipeReadFromParent[1]);

    for(int i=2; i<=35; i++){
        int t;
        int n = read(aPipeReadFromParent[0], &t, 4);
        //printf("pid %d read len %d\n", getpid(), n);
        if(n == 0){
            //printf("pid %d close write\n", getpid());
            close(aPipeWriteToChild[1]);
            break;
        }else{
            if(nFirstRead == -1){
                printf("PID:%d, prime:%d| %d, %d, %d, %d|, pp %d, p %d\n", getpid(), t, \
                aPipeWriteToChild[0], aPipeWriteToChild[1], \
                aPipeReadFromParent[0],aPipeReadFromParent[1], \
                nPPID, nPID);
                nFirstRead = t;
                continue;
            }
            if(i == 35){
                //printf("root reach\n");
                //close(aPipeReadFromParent[1]);//root
                //close(aPipeReadFromParent[0]);//root
            }

            if(t%nFirstRead == 0){
                continue;//drop
            }else{
                if(bForked){
                    write(aPipeWriteToChild[1], &t, 4);
                }else{
                    if(fork() != 0){
                        write(aPipeWriteToChild[1], &t, 4);
                        bForked = 1;
                    }else{
                        nPPID = nPID;
                        nPID = getpid();

                        i = 1;
                        bForked = 0;
                        nFirstRead = -1;
                        
                        //close(aPipeReadFromParent[0]);
                        //close(aPipeReadFromParent[1]);
                        aPipeReadFromParent[0] = aPipeWriteToChild[0];
                        aPipeReadFromParent[1] = aPipeWriteToChild[1];
                        
                        close(aPipeReadFromParent[1]);

                        aPipeWriteToChild[0] = -1;
                        aPipeWriteToChild[1] = -1;
                        pipe(aPipeWriteToChild);
                    }
                }
            } 
        }
    }
    close(aPipeWriteToChild[1]);
    //printf("pid %d close write, %d\n", getpid(), aPipeWriteToChild[1]);
    wait(0);
    //printf("pid %d close write\n", getpid());
    exit(0); 
}