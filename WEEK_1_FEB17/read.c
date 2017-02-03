#include<stdio.h>
//I,O system calls
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
//std unix libs
#include<unistd.h>
#include<stdlib.h>
//error handeling 
#include<errno.h>
#include<string.h>

//for catching errors
int err;


#define BUF_SIZE 1024
char buff[BUF_SIZE];


	
int main()
{
	//for file desc.
	int fd;

	fd=open("abc.txt",O_APPEND,S_IRUSR);
		if(fd==-1)
		{
			fprintf(stderr,"\nOpen failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		}
	printf("\nSystem call success:fd=%d \n",fd);
	
	
	err=read(fd,buff,BUF_SIZE);
		if(err==-1)
		{
			fprintf(stderr,"\nread failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		}
		
	printf("FILE READ:: %s",buff);
	
	
	
	printf("\nNOW closing\n");
	
	err=close(fd);
		if(err==-1)
		{
			fprintf(stderr,"\nclose failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		}





return 0;
}
