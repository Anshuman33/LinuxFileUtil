#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

char BUFF[1024];
int main(int argc, char * argv[]){
    int fd1 = open("hello.txt",O_CREAT | O_TRUNC | O_WRONLY);
    if(fd1 == -1)
        exit(1);
    char * s = "Hello World. I am Anshuman.";
    lseek(fd1, 10, SEEK_CUR);
    if(write(fd1, s, strlen(s))){
        printf("Write Success\n");
    }
    close(fd1);
    int fd2 = open("hello.txt", O_RDONLY);
    if(fd2 == -1)
        exit(1);
    struct stat mystat;
    fstat(fd2, &mystat);
    int readCount = 0;
    // while(readCount < mystat.st_size){
    //     int c;
    //     while((c = read(fd2, BUFF, sizeof(BUFF))) == 0)
    // }
    long int curroff = lseek(fd2, 0, SEEK_CUR);
    printf("\nCurrent offset:%ld", curroff);
    if((readCount = read(fd2, BUFF, sizeof(BUFF)))){
        printf("\nOffset after read:%ld", lseek(fd2, 0, SEEK_CUR));
        printf("Data Read:");
        for(int i = 0; i < readCount; i++){
            printf("%c",BUFF[i]);
        }
    }
    close(fd2);

}