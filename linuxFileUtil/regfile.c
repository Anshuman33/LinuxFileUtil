#include "regfile.h"
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>


int createFile(const char * path, mode_t mode){
    int fd = creat(path, mode);
    if(fd == -1){
        if(errno == EPERM || errno == EACCES)
            perror("\nPermission denied.");
        else if(errno == ENONET)
            perror("\nPath specified does not exist.");
        else if(errno == EISDIR)
            perror("\nTarget path is an existing directory.");
        else
            perror("\nError creating file");
    }
    return fd;
}

int openFile(const char * path, int mode){
    int fd = open(path, mode);
    if(fd == -1){
        if(errno == EPERM || errno == EACCES)
            perror("\nPermission denied.");
        else if(errno == ENONET)
            perror("\nPath specified does not exist.");
        else if(errno == EISDIR)
            perror("\nTarget path is an existing directory.");
        else
            perror("\nError opening file");
    }
    
}