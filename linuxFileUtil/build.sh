gcc -c fileutil.c
gcc -c helper.c
gcc -c main.c
gcc -o main main.o helper.o fileutil.o