//program to display accepted args

#include<stdio.h>
#include<getopt.h>
#include<unistd.h>
#include<stdlib.h>

// copy --help --version


static struct option opt_arr[]=
{
	{"help",no_argument,0,'h'},
	{"version",no_argument,0,'v'},
	{0,0,0,0}
};

void print_help();
void print_version();

int i=0;
int ret; //to catch return value of getopt

int main(int argc,char *argv[])
{
	while((ret=getopt_long(argc,argv,"hv",opt_arr,&i))!= -1)
	{
		switch((char)ret)
		{
		case 'h':
				print_help();
				break;
		case 'v':
				print_version();
				break;
		}
	}
	
	
	return 0;
	exit(EXIT_SUCCESS);
}



void print_help()
{
	printf("\nThis has --help -h  --version -v option\n");
}
void print_version()
{
	printf("\nThis is version 0.1\n");
}