#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>
#include<string.h>
#include<sys/uio.h>

#define BUFF_SIZE 512

char buff[BUFF_SIZE];
char buff2[BUFF_SIZE];




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

	struct iovec iov[2];

	//filling members 
	iov[0].iov_base=buff;
	iov[0].iov_len=512;
	iov[1].iov_base=buff2;
	iov[1].iov_len=512;

	nb_read=readv(fd,iov,2);
	if(nb_read==-1)
	{
		fprintf(stderr, "readv failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	err=writev(fd,iov,2);
	if(err==-1)
	{
		fprintf(stderr, "writev failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

    
return 0;
}
