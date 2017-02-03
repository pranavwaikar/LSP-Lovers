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


int err;
	
int main()
{
	//for file desc.
	int fd;

	fd=open("abcd.txt",O_APPEND,S_IRUSR);
		if(fd==-1)
		{
			fprintf(stderr,"\nOpen failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		}
	printf("\nSystem call success:fd=%d \n",fd);
	printf("\nNOW closing\n");
	
	err=close(fd);
		if(fd==-1)
		{
			fprintf(stderr,"\nclose failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		}





return 0;
}
