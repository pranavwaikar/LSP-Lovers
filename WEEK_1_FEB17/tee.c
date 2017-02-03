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

typedef int flag_t;

flag_t s_used=FALSE;
flag_t d_used=FALSE;

char *src_file_name=NULL;
char *out_file_name=NULL;
char buff[BUFF_SIZE];
int ret;

int fd_src,fd_dest;
int err;
int nb_read;

void print_help();
void print_version();
void tee(char *,char *);

int main(int argc,char *argv[])
{
	while((ret=getopt(argc,argv,"hvs:d:"))!=-1)
	{
		switch(ret)
		{
		case 'h':
			print_help();
			break;
		case 'v':
			print_version();
			break;
		case 's':
			src_file_name=optarg;
			s_used=TRUE;
			break;
		case 'd':
			out_file_name=optarg;
			d_used=TRUE;
			break;
		
		}
	}
	if(s_used==TRUE && d_used==TRUE)
	{
		tee(src_file_name,out_file_name);
	}
	return 0;
	exit(EXIT_SUCCESS);
}


void tee(char *src_file_name,char *out_file_name)
{
//oopening both files
	fd_src=open(src_file_name,O_RDWR|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd_src==-1)
		{
			fprintf(stderr,"\nOpen failed:src_file_name=%s:%s:%s:%s:%d\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
	fd_dest=open(out_file_name,O_RDWR|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd_dest==-1)
		{
			fprintf(stderr,"\nOpen failed:src_file_name=%s:%s:%s:%s:%d\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}
		
		
		//while true
		
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
			fprintf(stderr,"\nWrite failed:src_file_name=%s:%s:%s:%s:%d\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
			exit(EXIT_FAILURE);
		}

}

void print_help()
{
	printf("\nThis is help. tee command use -s: -d: -hv options\n");
}


void print_version()
{
	printf("\nThis is version 0.1\n");
}






























