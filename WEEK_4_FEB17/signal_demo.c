
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>


void my_sig_handler_one(int signum);


int main()
{
	signal(SIGUSR1,&my_sig_handler_one);

	kill(getpid(),SIGUSR1);
	
return 0;
}

void my_sig_handler_one(int signum)
{
	printf("\n This is my_sig_handler_one responding to SIGUSR1\n");
	return ;
}
