#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


void exit_handler_one(void);
void exit_handler_two(void);

int main(void)
{

	atexit((void *)exit_handler_one);
	atexit((void *)exit_handler_two);

	
	printf("\nHello world\n");
return 0;
}

void exit_handler_one(void)
{

	printf("\nHello from  one\n");
}

void exit_handler_two(void)
{
	printf("\nHello from two\n");
}
