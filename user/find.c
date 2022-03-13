#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// type 1 directory
// type 2 normal file

void 
find(const char * path, const char * destFileName){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        exit(0);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(0);
    }

    if(st.type == T_FILE){
        return ;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;

        if(strcmp(de.name, ".") == 0)
            continue;
        
        if(strcmp(de.name, "..") == 0)
            continue;
        
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0; // 字符串结尾
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        char bufNewPath[512];
        memmove(bufNewPath, de.name, DIRSIZ);
        //printf("p: %s, %d\n", bufNewPath, de.inum);
        if(strcmp(de.name, destFileName) == 0){
            printf("%s\n", buf);
        }

        if(st.type == T_DIR){
            find(buf, destFileName);
        }
    }
    return ;
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf("usage:find <dirName> <fileName>\n");
    }

    char * path = argv[1];
    char * fileName = argv[2];

    find(path, fileName);
    exit(0);
}