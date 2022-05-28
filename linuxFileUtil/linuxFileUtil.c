#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "fileutil.h"

#define esc 27
/*
    The program should be run by using the following command:

        ./linuxFileUtil <action> filename -t <reg|npipe|pipe>  -m <mode>
        
*/

void flushBuffer(char buf[], ssize_t n){
    for(int i = 0; i < n; i++){
        buf[i] = '\0';
    }
}

void flushExtraNewLine(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char * argv[]){
    int fd;
    char * action = NULL, *filepath=NULL, *mode=NULL, *buf=NULL, *filetype = "reg";

    if(argc < 2){
        printf("Insufficient arguments. Please specify the action along with relevant arguments.\n");
        exit(1);
    }
    action = argv[1];
    if(argc < 3){
        printf("Please provide path of the file.");
        exit(1);
    }
    filepath = argv[2];

    /* For info command */
    if(strcmp(action, "info") == 0){
        char infoBuf[500];
        if(getFileInfo(filepath, infoBuf) != -1)
            printf("%s",infoBuf);
    }
    
    /* For create command */
    else if(strcmp(action, "create") == 0){
        // Processing the optional commandline arguments
        if(argc > 3){
            int idx = 3;

            // Processing file type argument
            if(strcmp(argv[idx],"-t") == 0){
                if(argc < 4){
                    printf("Insufficient arguments. Provide file type. Use\n \
                            \t'reg' for regular file\n \
                            \t'npipe' for named pipe\n \
                            \t'pipe' for unnamed pipe\n.");
                    exit(1);
                }
                filetype = argv[4];
                idx += 2;
            }

            // Processing mode argument
            if(idx < argc && strcmp(argv[idx], "-m")==0){
                idx++;
                if(idx >= argc){
                    printf("Insufficient arguments. Please provide file mode.\n");
                    exit(1);
                }
                mode = argv[idx];
            }
        }

        // Creation of regular and fifo files
        if(strcmp(filetype, "reg" ) == 0 || strcmp(filetype,"npipe") == 0){
            fd = createFile(filepath, filetype, mode);
            if(fd != -1){
                printf("File Successfully created.\n");
                close(fd);
                exit(0);
            }
            else{
                printf("File creation failed\n");
                exit(1);
            }
        }
        // Creation of pipes
        else if(strcmp(filetype, "pipe") == 0){
            /*TODO: Add code for unnamed pipe*/


        }
        else{
            printf("Invalid file type.");
            exit(1);
        }
        
    }

    /* For open command.*/
    else if(strcmp(action, "open") == 0){
        if(argc < 4){
            printf("Please provide the mode of opening \
            \t<r = read, w = write, wt = truncate write, a = append>.\n");
            exit(1);
        }
        mode = argv[3];
        fd = openFile(filepath, mode);
        if(fd == -1){
            fprintf(stderr, "Exiting\n");
            exit(1);
        }

        // Get info about the file to check the filetype
        struct stat statbuf; 
        fstat(fd, &statbuf);
        
        int offset = -1;

        // Take offset as input for random access.
        if(S_ISREG(statbuf.st_mode) && strcmp(mode, "a") != 0){
            printf("\nEnter the offset(from beginning) at which to read/write(-1 for no change): ");
            scanf("%d", &offset);
            flushExtraNewLine(); // to flush extra new line character from stdin     
        }


        if(strcmp(mode,"r") == 0){     // If file opened in read mode

            // Take count of bytes to read as input
            int n;
            printf("Enter no. of bytes to read(-1 for uptil EOF):");
            scanf("%d", &n);

            // Dynamically allocate buffer of sufficient size for reading
            if(n > 0)
                buf = (char*) malloc(sizeof(char) * (n+1)); // 1 extra byte to append '\0'
            else
                buf = (char*) malloc(sizeof(char) * (statbuf.st_size+1));


            if(buf == NULL){
                perror("Unable to allocate buffer.");
                exit(1);
            }

            int b = readFile(fd, buf, n, offset);
            
            if(b > 0){
                buf[b] = '\0'; // Adding null character at the end of buffer
                printf("\nData Read from file:\n%s\n", buf);
            }
        }
        else{ // If file opened in write/append mode

            // Dynamically allocate buffer of sufficient size for taking input
            size_t bufSize = 1024;
            buf = (char *) malloc(sizeof(char) * bufSize);
            if(buf == NULL){
                perror("Unable to allocate buffer.");
                exit(1);
            }
            printf("Input data to write to file(Press esc and enter to finish):\n");
            int n = getdelim(&buf, &bufSize, esc, stdin);
            writeToFile(fd, buf, n-1, offset);
        }

        free(buf);
        close(fd);

    }

}