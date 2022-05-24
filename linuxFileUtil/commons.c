#include "commons.h"
#include <string.h>
#include <stdio.h>

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

int  formatInfo(const struct stat * info, char * buff){
    int uid = info->st_uid;
    int gid = info->st_gid;
    
    char filetype[30];

    getFileType(info->st_mode, filetype);

    char permStr[11];
    getPermStr(info->st_mode, permStr);

    int n = sprintf(buff, 
        "\nSize: %ld\tBlocks:%ld\tFile Type: %s \
        \nAccess Permissions:%s\tUID:%d\tGID:%d \
        \nDev#:%ld\tInode#:%ld\tLinks:%ld\n",
        info->st_size, info->st_blocks,filetype, 
        permStr, uid, gid, 
        info->st_dev, info->st_ino, info->st_nlink
    );

    return n;
}

int getFileInfo(const char * path, char * buff){
    struct stat info;
    int n = stat(path, &info);
    if(n == 0){
        int s = formatInfo(&info, buff);
        return s;
    }
    return -1;
}