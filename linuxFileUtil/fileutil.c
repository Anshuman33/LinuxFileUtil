#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include "commons.h"
#include "fileutil.h"

/*
    Creates new file.
    Parameters:
        - path: path of the file to be created
        - filetype: type of file (regular or fifo/named pipe)
        - modeStr: describes mode/permissions of the file to be created
    Returns:
        - file descriptor corresponding to the newly created file, -1 in case of error.

*/
int createFile(const char * path, char * filetype, char * modeStr){
    int fd;
    mode_t mode = 0666;

    if(modeStr != NULL){
        mode = octalToMode(modeStr);
        if(mode == -1)
            return -1;
    }

    if(filetype == NULL || strcmp(filetype,"reg") == 0){
        mode |= S_IFREG;
        fd = creat(path, mode);
    }
    else if(strcmp(filetype,"npipe")){
        mode |= S_IFIFO;
        //fd = mknod(path, 010777, 0);
        fd = mknod(path, mode, 0);
    }
    else{
        fprintf(stderr, "Invalid filetype.\n");
        return -1;
    }


    if(fd == -1){
        perror("Error creating file");
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
        flag = O_WRONLY | O_APPEND;
    else{
        perror("Invalid open mode.");
        return -1;
    }
    int fd = open(path, flag);
    if(fd == -1){
        perror("Error opening file");
    }
    return fd;
}

int readFile(int fd, char * buff, ssize_t numBytes, off_t offset){
    mode_t mode = getFileMode(fd);
    if(mode == 0){
        perror("Error reading from file");
        return -1;
    }

    if(S_ISREG(mode) && offset >= 0){ // For random access in case of regular files
        lseek(fd, offset, SEEK_SET);
    }

    ssize_t bytesRead = 0;
    if(numBytes == -1){
        int c = 0, readsize = 1024;
        
        while((c = read(fd, buff, readsize)) > 0){
            buff += c;
            bytesRead += c;
        }
    }
    else{
        bytesRead = read(fd, buff, numBytes);
    }
    
    if(bytesRead == -1){
        perror("Error reading from file");
        return -1;
    }
    return bytesRead;
}

int writeToFile(int fd, char * buff, ssize_t numBytes, off_t offset){
    mode_t mode = getFileMode(fd);
    if(mode == 0){
        perror(RED "\nError writing to file\n" NC);
        return -1;
    }


    if(S_ISREG(mode) && offset >= 0){ // For random access in case of regular files
        lseek(fd, offset, SEEK_SET);
    }

    ssize_t bytesWritten = 0;
    int b = 0;
    while(bytesWritten < numBytes){ // Loop until all bytes have been written or error occurs
        b = write(fd, buff, numBytes);
        if(b < 0){
            perror(RED "Error writing to file" NC);
            return -1;
        }
        bytesWritten += b;
        buff += b;
    }
    
    return bytesWritten;
}

/*
    Fetches information regarding the file corresponding to the path.
    Parameters:
        - path: path of the file.
        - buff: buffer in which the output/info string will be stored. Should be large enough
                to store the output.
    Returns: length of the output in buff or -1 incase of error
*/
int getFileInfo(const char * path, char * buff){
    
    struct stat info;
    int n = stat(path, &info);
    if(n == 0){
        int s = formatFileStat(&info, buff);
        return s;
    }
    else{
        perror(RED "Error fetching file information" NC);
        return -1;
    }
}
