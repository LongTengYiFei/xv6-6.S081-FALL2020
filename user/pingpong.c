#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int aPipeParentToChild[2];
    int aPipeChildToParent[2];
    pipe(aPipeParentToChild);
    pipe(aPipeChildToParent);

    if(fork() != 0){
        char aReadBuff[128] = {0};
        write(aPipeParentToChild[1], "ping ping", 10);
        read(aPipeChildToParent[0], aReadBuff, 128);
        printf("parent %d: recived %s\n", getpid(), aReadBuff);
    }else{
        char aReadBuff[128] = {0};
        read(aPipeParentToChild[0], aReadBuff, 128);
        printf("child %d: recived %s\n", getpid(), aReadBuff);
        write(aPipeChildToParent[1], "pong pong", 10);
    }
    exit(0);
}