#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>



int main(void)
{

pid_t pid;
pid=fork();
int ret;

if(pid==0)
{
	printf("\nThis is a child\n");
	execl("/bin/ls","ls","-h","-l","/home/pranav",(char *)0);
}
else
{
	wait(&ret);
	printf("\nThis is parent\n");
}

return 0;
}
