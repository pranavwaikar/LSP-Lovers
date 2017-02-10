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
	int fd;
	int err;

	fd=open("abc.txt",O_CREAT|O_RDWR,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr, "first Open failed:%s\n",strerror(errno) );
	}
	printf("Now we are sure that abc.txt is available\n");
	printf("ie.either created by open or already exists\n");

	printf("now trying the EXCL flag ie exclusively create the file.\n");
	fd=open("abc.txt",O_CREAT|O_RDWR|O_EXCL,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr, "Second Open failed due to excl flag:%s\n",strerror(errno) );
	}

return 0;
}
