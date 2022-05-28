gcc -c fileutil.c
gcc -c commons.c
gcc -c linuxFileUtil.c
gcc -o linuxFileUtil linuxFileUtil.o commons.o fileutil.o