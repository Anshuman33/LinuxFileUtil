#include<sys/stat.h>

// Creates a new file and returns its file descriptor
extern int createFile(const char * path, mode_t mode);

// Opens a file and returns its file descriptor.
extern int openFile(const char * path, const char * mode);

// Read from file randomly/sequentially
extern int readFile(int fd, int offset, char * buff, int numBytes);

// Write to file randomly/sequentially
extern int writeToFile(int fd, int offset, char * buff, int numBytes);
