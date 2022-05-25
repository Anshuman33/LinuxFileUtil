#include "regfile.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


int createFile(const char * path, mode_t mode){
    
    int fd = creat(path, mode);
    if(fd == -1){
        perror("Error creating file");
        // if(errno == EPERM || errno == EACCES)
        //     perror("\nPermission denied.");
        // else if(errno == ENONET)
        //     perror("\nPath specified does not exist.");
        // else if(errno == EISDIR)
        //     perror("\nTarget path is an existing directory.");
        // else
        //     perror("\nError creating file");
    }
    return fd;
}

int openFile(const char * path, const char * mode){
    int flag;
    if(strcmp(mode,"r") == 0)
        flag = O_RDONLY;
    else if(strcmp(mode, "w") == 0)
        flag = O_WRONLY;
    else if(strcmp(mode, "wt") == 0)
        flag = O_WRONLY | O_TRUNC;
    else if(strcmp(mode, "a") == 0)
        flag = O_APPEND;
    int fd = open(path, flag);
    if(fd == -1){
        perror("Error opening file");
        // if(errno == EPERM || errno == EACCES)
        //     perror("\nPermission denied.");
        // else if(errno == ENONET)
        //     perror("\nPath specified does not exist.");
        // else if(errno == EISDIR)
        //     perror("\nTarget path is an existing directory.");
        // else
        //     perror("\nError opening file");

    }
    return fd;
}

