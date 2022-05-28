#include<sys/stat.h>

#define RED "\e[0;31m"
#define NC "\e[0m"

extern mode_t getFileMode(int fd);
extern mode_t octalToMode(char * octModeStr);
extern int formatFileStat(const struct stat * info, char * buff);


