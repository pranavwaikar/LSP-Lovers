#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<getopt.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/cdrom.h>


#define TRUE 0
#define FALSE 1

typedef int flag_t;


int val;


void print_help();
void print_version();
void sys_err(char *error_name);

void eject();

int main(int argc,char *argv[])
{

	while((val=getopt(argc,argv,"hvy"))!=-1)
	{
		switch(val)
		{
		case 'h':
			print_help();
			break;
		case 'v':
			print_version();
			break;
		case 'y':
			eject();
			break;
		default:
			printf("\nINVALID OPTION \n");
			exit(EXIT_FAILURE);
		}
	}

return 0;
exit(EXIT_FAILURE);
}



void print_help()
{
	printf("\nThis is help. Use h,v,y option\n Thank you!\n");
}

void print_version()
{
	printf("\nThis is version 0.1\n");
}

void sys_err(char *error_name)
{
	fprintf(stderr,"\nLine:%dFunction:%sFile:%s\n",__LINE__,__FUNCTION__,__FILE__);
	fprintf(stderr,"\n%s:%s\n",error_name,strerror(errno));
}


void eject()
{
	int fd,ret;
	fd=open("/dev/cdrom",O_RDONLY|O_NONBLOCK);
		if(fd==-1)
		{	
			sys_err("open:cdrom:");
			exit(EXIT_FAILURE);
		}	
	ret=ioctl(fd,CDROMEJECT);
		if(ret==-1)
		{
			sys_err("ioctl:");
			exit(EXIT_FAILURE);
		}
}
