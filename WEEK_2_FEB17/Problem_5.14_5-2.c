/*

Write a program that opens an existing file for writing with the O_APPEND flag, and
then seeks to the beginning of the file before writing some data. Where does the
data appear in the file? Why?

Answer:--The data appeared at the end of the file.

why? we used O_APPEND.
Documentation--
O_APPEND-->
The file is opened in append mode. Before each write(2), the file offset is positioned at the end of the file, as if with lseek(2).

*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>
#include<string.h>




int main()
{
	int fd,nb_read;
	int err;

	fd=open("abc.txt",O_CREAT|O_RDWR|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr, " Open failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	lseek(fd,0,SEEK_SET);
	err=write(fd,"xx",2);
	if(err==-1)
	{
		fprintf(stderr, "write failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

    
return 0;
}
