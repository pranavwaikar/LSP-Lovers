//program to display accepted args

#include<stdio.h>
#include<getopt.h>


int ret; //to catch return value of getopt

void print_help();
void print_version();

//  $:  cp -s src.txt -o out.txt
//  $: prog -h -v
//     getopt(argc,argv,"s:o:hv");

int main(int argc,char *argv[])
{
	while((ret=getopt(argc,argv,"hv"))!= -1)
	{
		switch((char)ret)
		{
		//for help section
		case 'h':
				print_help();
				break;
		//for version section
		case 'v':
				print_version();
				break;
		}
	}
}

void print_help()
{
	printf("\nHello,this is help, options are -h and -v\n");
}

void print_version()
{
	printf("\nThis is version 0.1\n");
}