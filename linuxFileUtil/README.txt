* DESCRIPTION *
This is a file utility tool which can create, read, write to/from a file, 
pipe or fifo file. It can also print the information regarding the file.

* USAGE *
Type the following command with appropriate arguments.
        ./main <action> -t <type> -p <permissions> -m <mode> filename

where 
	 <action> : action to perform
		"info"  - to get file statistics,
		"create"- to create new file or
		"open"  - to open an existing file

	 <type>   : type of file to create (valid only for "create" action)
		"reg"   - create a regular file(default),
		"npipe" - create a named pipe or
		"pipe"  - create an unnamed pipe

    <permissions> : octal string representation for permissions of a file (valid only for "create" action)
	For eg. "666"(default) - read & write permission to owner,group & others.

	 <mode>   : mode of opening (valid only for "open" action)
		"r"     - open a file in read mode
		"w"     - open a file in write mode
		"wt"    - open a file in truncate write mode
		"a"     - open a file in append mode

* FILE DESCRIPTION *
    - build.sh : script file to run to compile and make the executable.
    - main.c : Driver program of the utility
    - helper.c : Library consisting of helper functions to be used by the fileutil.c and main.c
    - fileutil.c : Library consisting of functions for various actions on file like info, open, create, read and write.
    - helper.h : Header file for helper.c
    - fileutil.h : Header file for fileutil.h



