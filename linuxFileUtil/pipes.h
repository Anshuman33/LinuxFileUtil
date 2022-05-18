#include <sys/stat.h>

extern int openNamedPipe(const char * path, mode_t mode);

extern void getNamedPipeData(int fd);