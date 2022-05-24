#include <sys/stat.h>

// Opens a named pipe
extern int openNPipe(const char * path, mode_t mode);

// Reads data from named pipe
extern void readNPipe(int fd);

// Writes data to named pipe
extern void writeNPipe(int fd, char * data, int n);

// Close named pipe
extern void closeNPipe(int fd);

// Opens an unnamed pipe
extern void openPipe(int fds[]);

// Reads data from unnamed pipe
extern void readPipe(int readfd);

// Writes data in unnamed pipe
extern void writePipe(int writefd, char * data, int n);

// Close an unnamed pipe
extern void closePipe(int fds[]);

