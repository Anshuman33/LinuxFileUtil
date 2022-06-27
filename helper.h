// Include guards
#ifndef COMMONS_H
#define COMMONS_H

#include<sys/stat.h>

#define RED "\e[0;31m"  // For red color
#define NC "\e[0m"     // For default color

mode_t getFileMode(int fd);
mode_t octalToMode(char * octModeStr);
int formatFileStat(const struct stat * info, char * buff);
int fileExists(const char * path);

#endif


