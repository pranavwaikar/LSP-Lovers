/*
Use:  $:cat abc.txt
      $:cat -h -v 
*/
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
flag_t h_used=FALSE;
flag_t v_used=FALSE;

int err;
int ret,i=0;	
int size=0;
int nb_read;
	
char *src_file_name=NULL;
char buff[BUFF_SIZE];


static struct option opts_arr[]=
{
	{"help",no_argument,0,'h'},
	{"version",no_argument,0,'v'},
	{0,0,0,0}
};
void print_help();
void print_version();
void cat(char *);


int main(int argc,char *argv[])
{
	while((ret=getopt_long(argc,argv,"s:hv",opts_arr,&i))!=-1)
	{
		switch(ret)
		{
		case 'h':
			h_used=TRUE;
			print_help();
			break;
		case 'v':
			v_used=TRUE;
			print_version();
			break;
		}
	}
	//accepting source file name
	src_file_name=argv[optind];
	
	if((h_used==TRUE || v_used==TRUE) && src_file_name==NULL)
	{
		exit(EXIT_SUCCESS);
	} 
	
	if(src_file_name==NULL)
	{
		fprintf(stderr,"\nFatal:Error:No input file specified\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		cat(src_file_name);
	}

return 0;
}


void cat(char *src_file_name)
{
int fd;


	fd=open(src_file_name,O_RDONLY);
	if(fd==-1)
	{
		fprintf(stderr,"\nOpen failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}
	//seeking to start of file
	err=lseek(fd,0,SEEK_SET);
	if(err==-1)
	{
		fprintf(stderr,"\nlseek failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}
	while(nb_read=read(fd,buff,BUFF_SIZE))
	{
		err=write(STDOUT_FILENO,buff,nb_read);
	}
	
	if(nb_read==-1)
	{
		fprintf(stderr,"\nRead failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}

	if(err==-1)
	{
		fprintf(stderr,"\nWrite failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}
	err=close(fd);
	if(err==-1)
	{
		fprintf(stderr,"\nClose failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}
}

void print_help()
{
static char buff[]="\nThis is help.genfile options are--\n\
			1)--help     or -h\n\
			2)--version  or -v\n\
			Thank you!\n";
	printf("%s",buff);
}

void print_version()
{
	printf("\nThis is version 0.1\n");
}
