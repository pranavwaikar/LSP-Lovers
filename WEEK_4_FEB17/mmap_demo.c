#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

void *address;

int main()
{
char *ptr2=NULL;
char *ptr1=NULL;
int err;
ptr1=(char *)malloc(PAGE_SIZE);

address=mmap(NULL,PAGE_SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	if(address==(void *)(-1))
		printf("\nmmap failed\n");
printf("\nadd=%p\n",address);
ptr2=(char *)malloc(PAGE_SIZE);
printf("\nptr1=%p   ptr2=%p\n",ptr1,ptr2);

err=munmap(address,PAGE_SIZE);
	if(err==-1)
		printf("\nmunmap() failed\n");

return 0;
exit(EXIT_SUCCESS);
}
