#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<fcntl.h>

int main()
{
	struct stat sb;
	int fd;
	int err;
	off_t start;
	fd=open("abc.txt",O_RDWR);


	err=stat("abc.txt",&sb);
	if(start==-1)
		fprintf(stderr, "stat failed\n" );
	printf("\nFile size before=%ld\n",(long)sb.st_size);

	//seek 100 bytes forward from end of file
	start=lseek(fd,100,SEEK_END);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nseek 100 bytes forward from end of file\n");
	fprintf(stdout, "offset=%ld\n",start );
	//file size after lseek
		err=stat("abc.txt",&sb);
	if(start==-1)
		fprintf(stderr, "stat failed\n" );
	printf("\nFile size after lseek=%ld\n",(long)sb.st_size);
	//write 1 byte
	err=write(fd,"x",1);
	if(err==-1)
		fprintf(stderr, "write failed\n" );
	//file size after lseek and write
		err=stat("abc.txt",&sb);
	if(start==-1)
		fprintf(stderr, "stat failed\n" );
	printf("\nFile size after write=%ld\n",(long)sb.st_size);


	

return 0;
}
