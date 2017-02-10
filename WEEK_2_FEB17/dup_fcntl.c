/*
Article 5.14
problem-5.4

Implement dup() and dup2() using fcntl() and, where necessary, close(). (You may
ignore the fact that dup2() and fcntl() return different errno values for some error
cases.) For dup2(), remember to handle the special case where oldfd equals newfd. In
this case, you should check whether oldfd is valid, which can be done by, for example,
checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not valid, then the function
should return –1 with errno set to EBADF .

*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>
#include<string.h>




int main()
{
	int fd,fd2,fd3;
	int err;

	fd=open("abc.txt",O_CREAT|O_RDWR|O_APPEND,S_IRWXU|S_IRGRP|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr, " Open failed:%s\n",strerror(errno) );
		exit(EXIT_FAILURE);
	}
	
	//exact copy of fd is created & value is tored in fd2.
	fd2=dup(fd);
	
	//exact copy of fd will be created at greater than or equal to fd2. result will be stored in fd3
	fd3=(fd,fd2);
	
	//redirected stdout to abc.txt
	dup2(fd,STDOUT_FILENO); 
	//following printf will appear in abc.txt not on terminal
	printf("\nNow this is end of program...\n");

	

    
return 0;
}


int dup(int oldfd) 
{
    return fcntl(oldfd, F_DUPFD, STDERR_FILENO);
}

int dup2(int oldfd, int newfd) 
{

    if(oldfd == newfd)
        return oldfd;

    if(fcntl(oldfd, F_GETFD) == -1)
        return -1;

    if(close(newfd) == -1 && errno != EBADF)
        return -1;

    fcntl(oldfd, F_DUPFD, newfd);

    return newfd;
}
