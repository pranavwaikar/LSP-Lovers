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
#include<getopt.h>


#define TRUE  0
#define FALSE 1
#define BUFF_SIZE 1024



char *src_file_name=NULL;
char *out_file_name=NULL;
char buff[BUFF_SIZE];
int ret;

int fd_src,fd_dest;
int err;
int nb_read;

void print_help();
void print_version();
void copy(char *,char *);

int main(int argc,char *argv[])
{
	while((ret=getopt(argc,argv,"hv"))!=-1)
	{
		switch(ret)
		{
		case 'h':
			print_help();
			break;
		case 'v':
			print_version();
			break;
		
		}
	}
	src_file_name=argv[optind];
	out_file_name=argv[optind+1];
	
	
	if(src_file_name==NULL || out_file_name==NULL)
	{
		fprintf(stderr,"\nFatal:Error:Input error\n");
	}
	else
	{
		copy(src_file_name,out_file_name);
	}
	return 0;
	exit(EXIT_SUCCESS);
}


void copy(char *src_file_name,char *out_file_name)
{
//opening both files
	fd_src=open(src_file_name,O_RDONLY,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd_src==-1)
		{
			fprintf(stderr,"\nOpen failed:src_file_name=%s:%s:%s:%s:%d\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		
		
	fd_dest=open(out_file_name,O_CREAT|O_RDWR|O_APPEND,S_IRWXU|S_IRGRP|S_IWGRP|S_IROTH);
	if(fd_dest==-1)
		{
			fprintf(stderr,"\nOpen failed:out_file_name=%s:%s:%s:%s:%d\n",out_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		
		
		//while not end of file
		
		while((nb_read=read(fd_src,buff,BUFF_SIZE))!=0)
		{
			err=write(fd_dest,buff,nb_read);
		}
		
		if(nb_read==-1)
		{
			fprintf(stderr,"\nread failed:src_file_name=%s:%s:%s:%s:%d\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		
		if(err==-1)
		{
			fprintf(stderr,"\nWrite failed:out_file_name=%s:%s:%s:%s:%d\n",out_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		//now closing files
		err=close(fd_src);
		if(err==-1)
		{
			fprintf(stderr,"\nClose failed:src_file_name=%s:%s:%s:%s:%d\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		err=close(fd_dest);
		if(err==-1)
		{
			fprintf(stderr,"\nWrite failed:out_file_name=%s:%s:%s:%s:%d\n",out_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}

}

void print_help()
{
	printf("\nThis is help. copy command use -h -v options\n");
	printf("\nuse: $:copy src_file dest_file\n");
}


void print_version()
{
	printf("\nThis is version 0.1\n");
}




