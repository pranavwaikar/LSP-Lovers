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
int fd;

	
int main()
{
	//for file desc.
	
	int nb_read;

	fd=open("abc.txt",O_APPEND|O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO);
		if(fd==-1)
		{
			fprintf(stderr,"\nOpen failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
	printf("\nSystem call success:fd=%d \n",fd);
	
	
	nb_read=read(fd,buff,BUF_SIZE);
	err=nb_read;
		if(err==-1)
		{
			fprintf(stderr,"\nread failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		
	printf("FILE READ:: %s",buff);
	
	//print in file
	err=write(fd,buff,nb_read);
		if(err==-1)
		{
			fprintf(stderr,"\nwrite failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
	//print on terminal
		err=write(STDOUT_FILENO,buff,nb_read);
		if(err==-1)
		{
			fprintf(stderr,"\nwrite failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
	printf("\nWrite successful\n");
	
	printf("\nNOW closing\n");
	
	err=close(fd);
		if(err==-1)
		{
			fprintf(stderr,"\nclose failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}





return 0;
}
