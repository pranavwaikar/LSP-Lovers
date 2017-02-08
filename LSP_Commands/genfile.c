/*
Use:  $:genfile --size=1024 abc.txt
      $:genfile -h -v 
      $:genfile abc.txt	
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

typedef int flag_t;
flag_t h_used=FALSE;
flag_t v_used=FALSE;

int err;
int ret,i=0;	
int size=0;
	
char *src_file_name=NULL;


static struct option opts_arr[]=
{
	{"help",no_argument,0,'h'},
	{"version",no_argument,0,'v'},
	{"size",required_argument,0,'s'},
	{0,0,0,0}
};
void print_help();
void print_version();
void genfile(char *,int);


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
		case 's':
			size=atoi(optarg);
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
		genfile(src_file_name,size);
	}

return 0;
}


void genfile(char *src_file_name,int size)
{
int fd,i;
long int buff[size/sizeof(long int)];


	fd=open(src_file_name,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr,"\nOpen failed:%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}
	
	//seed of random number 
	srand(time(0));
	
	for(i=0;i<=size/sizeof(long int);i++)
	{
		buff[i]=rand();
	}

	err=write(fd,buff,sizeof(buff));
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
			3)--size     or -s\n\
			Thank you!\n";
	printf("%s",buff);
}

void print_version()
{
	printf("\nThis is version 0.1\n");
}
