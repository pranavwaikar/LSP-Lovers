#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 1024

char buff[BUFF_SIZE];

int main()
{
	int fd;
	off_t err;

	fd=open("abc.txt",O_CREAT|O_RDWR,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr, "first Open failed:%s\n",strerror(errno) );
	}

	//#######pread############


	//setting offset to start of file
	err=lseek(fd,0,SEEK_SET);
	if(err==-1)
	{
		fprintf(stderr, "lseekfailed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	printf("offset before pread=%ld\n",(long)err);
	//calling pread

	err=pread(fd,buff,BUFF_SIZE,100);
	if(err==-1)
	{
		fprintf(stderr, "pread failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	printf("\nAfter pread--100 bytes form start--buff=\n%s\n",buff);
	//after pread offset value
	err=lseek(fd,0,SEEK_CUR);
	if(err==-1)
	{
		fprintf(stderr, "lseekfailed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}
	printf("offset after pread=%ld\n",(long)err);

	printf("\npread demo complete\n");

	//##########pwrite##################
	printf("\npwrite demo start\n");
	//setting offset to start of file
	err=lseek(fd,0,SEEK_SET);
	if(err==-1)
	{
		fprintf(stderr, "lseekfailed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	printf("offset before pread=%ld\n",(long)err);
	//calling pread

	err=pread(fd,buff,BUFF_SIZE,100);
	if(err==-1)
	{
		fprintf(stderr, "pread failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	printf("\nAfter pread--100 bytes form start--buff=\n%s\n",buff);


	//writing word--GODREJ at offset 100
	err=pwrite(fd,"GODREJ",6,100);
	if(err==-1)
	{
		fprintf(stderr, "pread failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}
	//buffer after pwrite
	err=pread(fd,buff,BUFF_SIZE,100);
	if(err==-1)
	{
		fprintf(stderr, "pread failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}

	printf("\nAfter pwrite--100 bytes form start--buff=\n%s\n",buff);
	//after pwrite offset value
	err=lseek(fd,0,SEEK_CUR);
	if(err==-1)
	{
		fprintf(stderr, "lseekfailed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}
	printf("offset after pwrite=%ld\n",(long)err);

	printf("pwrite demo complete\n");

	printf("\nhence proved that pread/pwrite perform operation at specified offset ");
	printf("without changing the entry of offset in file table\n");
    
return 0;
}
