#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>


int main(int argc, char const *argv[])
{
	if(argc!=2)
		exit(-1);
	DIR *dir;
	struct dirent *entry;

	dir=opendir(argv[1]);

	while( (entry=readdir(dir)) !=NULL)
	{
		printf("\nDirectory name=%s   ",entry->d_name);
		printf("Inode no=%ld\n",(long)entry->d_ino);
	}

	closedir(dir);

	return 0;
}
