#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>



int main(void)
{

pid_t pid;
int ret;
int x;

pid=fork();

if(pid==0)
{
	printf("This is child\n");
	exit(EXIT_SUCCESS);
}
else
{
	wait(&ret);
	printf("\nPARENT\n");
}

x= (int)  (* ( (char *)  (((char *)&ret) +1) )) ;   //exit status is in second last position, extracting second last 2 bytes here
printf("\nExit status:%d\n",x);


int y,z=20;
y=*(&z);	// value at address z

printf("\n%d\t%d\t%d\n",y,z,y);

return 0;
exit(EXIT_SUCCESS);
}
