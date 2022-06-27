#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "helper.h"

/*
    Get the mode field of the file corresponding to the file descriptor.
    Parameters:
        - fd: file descriptor of the file
    Returns:
        - mode field of the file.
*/
mode_t getFileMode(int fd){
    struct stat statbuf;
    if(fstat(fd, &statbuf) == -1){
        return 0;
    }
    return statbuf.st_mode;
}

/*
    Convert octal string of file permissions to mode_t.
    Parameters:
        - octModeStr: permissions of the file in octal string form.
    Returns:
        - mode_t with permissions specified in octal string.
*/
mode_t octalToMode(char * octModeStr){
    
    if(strlen(octModeStr) != 3){
        fprintf(stderr, "Invalid mode string.\n");
        return -1;
    }
    mode_t mode = 0;
    
    unsigned int usrPerm = octModeStr[0] - '0';
    unsigned int grpPerm = octModeStr[1] - '0';
    unsigned int othPerm = octModeStr[2] - '0';
    if(usrPerm > 7 || grpPerm > 7 || othPerm > 7){
        fprintf(stderr, "Invalid mode string %d%d%d.\n",usrPerm,grpPerm,othPerm);
        return -1;
    }
    if(usrPerm & 4)
        mode |= S_IRUSR;
    if(usrPerm & 2)
        mode |= S_IWUSR;
    if(usrPerm & 1)
        mode |= S_IXUSR;
    if(grpPerm & 4)
        mode |= S_IRGRP;
    if(grpPerm & 2)
        mode |= S_IWGRP;
    if(grpPerm & 1)
        mode |= S_IXGRP;
    if(othPerm & 4)
        mode |= S_IROTH;
    if(othPerm & 2)
        mode |= S_IWOTH;
    if(othPerm & 1)
        mode |= S_IXOTH;
    return mode;
}

/*
    Fetches file permissions from mode field and
    converts into UNIX style permission string.
    Parameters: 
        mode: mode field of a file
        buf: buffer to store the file type string
    
    Returns: none
*/
void getPermStr(mode_t mode, char * buf){

    // Fill all positions with '-' initially
    for(int i = 0; i < 10; i++){
        buf[i] = '-';
    }
    // Set first character for directory or pipe
    if((mode & S_IFMT) == S_IFIFO)
        buf[0] = 'p';
    else if((mode & S_IFMT) == S_IFDIR)
        buf[0] = 'd';
    else if((mode & S_IFMT) == S_IFLNK)
        buf[0] = 'l';
        
    
    // Set permissions characters for user
    if(mode & S_IRUSR)
        buf[1] = 'r';
    if(mode & S_IWUSR)
        buf[2] = 'w';
    if(mode & S_IXUSR)
        buf[3] = 'x';

    // Set permissions characters for group
    if(mode & S_IRGRP)
        buf[4] = 'r';
    if(mode & S_IWGRP)
        buf[5] = 'w';
    if(mode & S_IXGRP)
        buf[6] = 'x';

    // Set permissions characters for others
    if(mode & S_IROTH)
        buf[7] = 'r';
    if(mode & S_IWOTH)
        buf[8] = 'w';
    if(mode & S_IXOTH)
        buf[9] = 'x';
    buf[10] = '\0';
}

/*
    Fetches type of file from mode field.
    Parameters: 
        mode: mode field of a file
        buf: buffer to store the file type string
    
    Returns: none
*/
void getFileType(mode_t mode, char * buf){
    
    if((mode & S_IFMT) == S_IFREG)
        sprintf(buf, "regular file");
    else if((mode & S_IFMT) == S_IFDIR)
        sprintf(buf, "directory");
    else if((mode & S_IFMT) == S_IFIFO)
        sprintf(buf, "fifo pipe");
    else if((mode & S_IFMT) == S_IFLNK)
        sprintf(buf, "symbolic link");
    else if((mode & S_IFMT) == S_IFBLK)
        sprintf(buf, "block special file");
    else if((mode & S_IFMT) == S_IFCHR)
        sprintf(buf, "character special file");
}

/*
    Converts output returned from stat system call into readable format.
    Parameters:
        - info: pointer to struct stat containing the file information returned from stat().
        - buff: buffer to store the formatted string.
    Returns:
        number of characters written onto buffer.
*/
int formatFileStat(const struct stat * info, char * buff){
    int uid = info->st_uid;
    int gid = info->st_gid;
    
    char filetype[30];

    getFileType(info->st_mode, filetype);

    char permStr[11];
    getPermStr(info->st_mode, permStr);

    char * ctimstr = ctime(&(info->st_ctim.tv_sec));
    char * atimstr = ctime(&(info->st_atim.tv_sec));
    char * mtimstr = ctime(&(info->st_atim.tv_sec));
    int n = sprintf(buff, 
        "\nSize: %ld\tBlocks:%ld\tFile Type: %s "
        "\nAccess Permissions:%s\tUID:%d\tGID:%d "
        "\nDev#:%ld\tInode#:%ld\tLinks:%ld "
        "\nInode change time: %s"
        "Last access: %s"
        "Last modified: %s\n",
        info->st_size, info->st_blocks,filetype, 
        permStr, uid, gid, 
        info->st_dev, info->st_ino, info->st_nlink,
        ctimstr,
        atimstr,
        mtimstr
    );

    return n;
}

/*
    Checks if file exists
    Parameters:
        path: path of the file to be tested
    Returns:
        1 if file exists, else 0
    
*/
int fileExists(const char * path){
    int fd = open(path, O_RDONLY);
    if(fd == -1 && errno == ENOENT){
        return 0;
    }
    close(fd);
    return 1;
}
