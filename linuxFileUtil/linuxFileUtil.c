#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include "commons.h"
#include "regfile.h"
#include <fcntl.h>
#include <stdlib.h>
/*
    The program should be run by using the following command:
        linuxFileUtil.c arg1 arg2 ...

        linuxFileUtil <action> -t <regfile|pipe|unnamed pipe> filename mode
        Eg.
            myutil open filename mode
            myutil info filename
            myutil create -t regfile filename --mode <creation_mode>

    >> arg1 : whether to display info for an existing file or open an existing file or create a new file
    >> arg1 : type of file for a new file
    >> arg2 : path of file
    >> arg3 : mode of opening (r,w,rw)
    >> arg4 : 

    - Check whether appropriate permission is allowed
    - If mode == write, prompt user for content.
    - If mode == read, display the content.

     
*/
int main(int argc, char * argv[]){
    char * action, *filepath;
    if(argc < 2){
        printf("Insufficient arguments. Please specify the action along with relevant arguments.\n");
        exit(1);
    }
    action = argv[1];
    if(strcmp(action, "info") == 0){
        if(argc < 3){
            printf("Please provide path of the file.");
            exit(1);
        }
        filepath = argv[2];
        char infoBuf[500];
        if(getFileInfo(filepath, infoBuf) != -1)
            printf("%s",infoBuf);
    }
    else if(strcmp(action, "open") == 0){
        if(argc < 3){
            printf("Please provide path of the file.\n");
            exit(1);
        }
        if(argc < 4){
            printf("Please provide the mode of opening <r, w, a>.\n");
            exit(1);
        }
        filepath = argv[2];
        char * mode = argv[3];
        int fd = openFile(filepath, mode);
        if(fd == -1){
            fprintf(stderr, "Exiting\n");
            exit(1);
        }
        // File opened succesfully
        struct stat statbuf;
        fstat(fd, &statbuf);

        if(S_ISFIFO(statbuf.st_mode)){
            // If opened for read mode
            if(strcmp(mode,"r") == 0){
                int n;
                printf("Enter no. of bytes to read:");
                scanf("%d", &n);
                /*Resume coding from here.*/
            }
            
        }
        else if(S_ISREG(statbuf.st_mode)){

        }
        close(fd);

    }

}