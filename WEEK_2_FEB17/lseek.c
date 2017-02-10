#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<getopt.h>
#include<fcntl.h>

int main()
{
	int fd;
	off_t start;
	fd=open("abc.txt",O_RDWR);

	//set the seek to 0 ie.start of file
	start=lseek(fd,0,SEEK_SET);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nset the seek to 0 ie.start of file\n");
	fprintf(stdout, "offset=%ld\n",start );

	//set the seek to end of file
	start=lseek(fd,0,SEEK_END);
	if(start==-1)
		fprintf(stderr, "lseek failed\n");
	printf("\nset the seek to end of file\n");
	fprintf(stdout, "offset=%ld\n",start );

	//seek 100 bytes forward from end of file
	start=lseek(fd,100,SEEK_END);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nseek 100 bytes forward from end of file\n");
	fprintf(stdout, "offset=%ld\n",start );

	//seek 10 bytes backwards from end of file
	start=lseek(fd,-10,SEEK_END);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nseek 10 bytes backwards from end of file\n");
	fprintf(stdout, "offset=%ld\n",start );

	//seek 20 bytes froward from currrent offset
	start=lseek(fd,20,SEEK_CUR);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nseek 20 bytes froward from currrent offset\n");
	fprintf(stdout, "offset=%ld\n",start );

	//seek 30 bytes backward from current offset
	start=lseek(fd,-30,SEEK_CUR);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nseek 30 bytes backward from current offset\n");
	fprintf(stdout, "offset=%ld\n",start );

	//seek to 50th byte ie from start of file
	start=lseek(fd,50,SEEK_SET);
	if(start==-1)
		fprintf(stderr, "lseek failed\n" );
	printf("\nseek to 50th byte ie from start of file\n");
	fprintf(stdout, "offset=%ld\n",start );

return 0;
}
