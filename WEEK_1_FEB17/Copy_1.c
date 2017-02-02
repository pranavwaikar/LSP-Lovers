//program to copy framework

#include<stdio.h>
#include<getopt.h>
#include<unistd.h>
#include<stdlib.h>

// copy -s srcfile -o outfile

#define TRUE  0
#define FALSE 1

typedef int flag_t;
flag_t s_used=FALSE;
flag_t o_used=FALSE;

void copy(char *,char *);
void print_help();
void print_version();


char *src_file_name=NULL;
char *out_file_name=NULL;
int ret; //to catch return value of getopt

int main(int argc,char *argv[])
{
	while((ret=getopt(argc,argv,"s:o:hv"))!= -1)
	{
		switch((char)ret)
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
		case 'o':
				out_file_name=optarg;
				o_used=TRUE;
				break;
		}
	}
	
	if(s_used == TRUE && o_used==TRUE)
	{
		copy(src_file_name,out_file_name);
	}
	return 0;
	exit(EXIT_SUCCESS);
}

void copy(char *src_file_name,char *out_file_name)
{
	printf("\nWe will copy from here\n");
	printf("\nsrc name=%s  out_name=%s\n",src_file_name,out_file_name);
}

void print_help()
{
	printf("\nThis has -s: -o: -h -v option\n");
}
void print_version()
{
	printf("\nThis is version 0.1\n");
}