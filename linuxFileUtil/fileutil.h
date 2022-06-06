#include<sys/stat.h>

// Fetches information regarding the file.
extern int getFileInfo(char * path, char * buff);

// Creates a new file and returns its file descriptor
extern int createFile(const char * path, char * filetype, char * mode);

// Opens a file and returns its file descriptor.
extern int openFile(const char * path, const char * mode);

// Read from file randomly/sequentially
extern int readFile(int fd, char * buff, ssize_t numBytes, off_t offset);

// Write to file randomly/sequentially
extern int writeToFile(int fd, char * buff, ssize_t numBytes, off_t offset);

