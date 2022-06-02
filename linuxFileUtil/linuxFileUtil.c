#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "fileutil.h"
#include "commons.h"

#define esc 27
/*
    The program should be run by using the following command:

        ./linuxFileUtil <action> filename -t <reg/npipe/pipe>  -m <mode>
        
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
    off_t offset;
    ssize_t byteCount, bufsize;
    struct stat statbuf;
    char * action = NULL, *filepath=NULL, *mode=NULL, *buf=NULL, *filetype = "reg", *octalStr=NULL;

    if(argc < 2){
        printf("Insufficient arguments. Please specify the action along with relevant arguments.\n");
        printf("Usage:\n");
        printf("\t./linuxFileUtil <action> filename -t <type> -p <permissions> -m <mode>\n");
        printf(
            "\nwhere "
            "\n\t <action> : action to perform"
            "\n\t\t\"info\"  - to get file statistics,"
            "\n\t\t\"create\"- to create new file or" 
            "\n\t\t\"open\"  - to open an existing file\n"
            "\n\t <type>   : type of file to create (valid only for \"create\" action)"
            "\n\t\t\"reg\"   - create a regular file(default),"
            "\n\t\t\"npipe\" - create a named pipe or"
            "\n\t\t\"pipe\"  - create an unnamed pipe\n"
            "\n    <permissions> : octal string representation for permissions of a file (valid only for \"create\" action)"
            "\n\tFor eg. \"666\"(default) - read & write permission to owner,group & others.\n"
            "\n\t <mode>   : mode of opening (valid only for \"open\" action)"
            "\n\t\t\"r\"     - open a file in read mode"
            "\n\t\t\"w\"     - open a file in write mode"
            "\n\t\t\"wt\"    - open a file in truncate write mode"
            "\n\t\t\"a\"     - open a file in append mode"
            "\n"
        );

        exit(1);
    }
    action = argv[1];
    if(argc < 3){
        printf("Please provide path of the file.\n");
        exit(1);
    }
    filepath = argv[2];

    // Processing the optional commandline arguments
    if(argc > 3){
        int idx = 3;
        while(idx < argc){
            // Processing file type argument
            if(strcmp(argv[idx],"-t") == 0){
                idx++;
                if(idx >= argc){
                    printf("Insufficient arguments. Provide file type. Use\n \
                            \t'reg' for regular file\n \
                            \t'npipe' for named pipe\n \
                            \t'pipe' for unnamed pipe\n.");
                    exit(1);
                }
                filetype = argv[idx];
                idx++;
            }

            // Processing permissions argument
            else if(strcmp(argv[idx], "-p")==0){
                idx++;
                if(idx >= argc){
                    printf("Insufficient arguments. Please provide octal string for permission.\n");
                    exit(1);
                }
                octalStr = argv[idx];
                idx++;
            }

            // Processing mode argument
            else if(strcmp(argv[idx], "-m")==0){
                idx++;
                if(idx >= argc){
                    printf("Insufficient arguments. Please provide file mode.\n");
                    exit(1);
                }
                mode = argv[idx];
                idx++;
            }

            else
                idx++;
        }
        
    }

    /* For info command */
    if(strcmp(action, "info") == 0){
        char infoBuf[500];
        if(getFileInfo(filepath, infoBuf) != -1)
            printf("%s",infoBuf);
    }
    
    /* For create command */
    else if(strcmp(action, "create") == 0){
        

        // Creation of regular and fifo files
        if(strcmp(filetype, "reg" ) == 0 || strcmp(filetype,"npipe") == 0){
            
            if(fileExists(filepath)){
                printf("\nFile already exists. Do you want to truncate it?(y/n):");
                char res;
                scanf("%c", &res);
                flushExtraNewLine();
                if(res != 'y'){
                    printf("\nAbort file creation\n");
                    exit(1);
                }
            }
            fd = createFile(filepath, filetype, octalStr);
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
        fd = openFile(filepath, mode);
        if(fd == -1){
            fprintf(stderr, "Exiting\n");
            exit(1);
        }

        // Get info about the file to check the filetype
        fstat(fd, &statbuf);
        

        // Handling of regular files
        if(S_ISREG(statbuf.st_mode)){

            // Take offset as input for random access.
            offset = -1;
            if(strcmp(mode, "a") != 0){
                printf("\nEnter the offset(from beginning) at which to read/write(-1 for no change): ");
                scanf("%ld", &offset);
                flushExtraNewLine(); // to flush extra new line character from stdin  
            }

            if(strcmp(mode, "r") == 0){
                // Take count of bytes to read as input
                printf("Enter no. of bytes to read(-1 for uptil EOF):");
                scanf("%ld", &bufsize);

                // Dynamically allocate buffer of sufficient size for reading
                if(bufsize <= 0)
                    bufsize = statbuf.st_size;
                
                buf = (char*) malloc(sizeof(char) * (bufsize));
                
                if(buf == NULL){
                    perror("Unable to allocate buffer.");
                    exit(1);
                }

                byteCount = readFile(fd, buf, bufsize, offset);
                
                if(byteCount >= 0){
                    printf("\nData Read from file:\n");
                    for(int i = 0 ; i < byteCount; i++){
                        printf("%c", buf[i]);
                    }
                    printf("\n");
                }
            }
            else{
                bufsize = 0;
                buf = NULL;
                printf("Input data to write to file(Press esc and enter to finish):\n");
                bufsize = getdelim(&buf, &bufsize, esc, stdin);
                writeToFile(fd, buf, bufsize-1, offset);
            }
        } 
        else if(S_ISFIFO(statbuf.st_mode)){ // Handling of fifo files
            if(strcmp(mode, "r") == 0){
                bufsize = 1024;
                buf = (char*) malloc(sizeof(char) * bufsize); 

                if(buf == NULL){
                    perror("Unable to allocate buffer.");
                    exit(1);
                }

                while(1){
                    byteCount = readFile(fd, buf, bufsize, -1);
                    
                    if(byteCount > 0){
                        printf("\nData Read from file:\n");
                        for(int i = 0 ; i < byteCount; i++){
                            printf("%c", buf[i]);
                        }
                        printf("\n");
                    }
                }
            }
            else{
                bufsize = 0;
                buf = NULL;
                while(1){
                    printf("Input data to write to file(Press enter without data to exit):\n");
                    bufsize = getline(&buf, &bufsize, stdin);
                    if(bufsize-1 > 0) // using bufsize-1 characters to remove the '\n' character.
                        writeToFile(fd, buf, bufsize-1, -1);
                    else
                        break;   
                }
            }
        }
        if(buf != NULL)
            free(buf);
            
        close(fd);
    }
    else{
        printf("Invalid action.\n");
        exit(1);
    }

}