
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





int main(int argc,char *argv[])
{
	if (argc!=2)
	{
		printf("Please enter file name\n");
		exit(EXIT_FAILURE);
	}
 char *value;
 char buff[256];
 
    value = "The is my attribute.";
    if (setxattr(argv[1], "user.x", value, strlen(value), 0) == -1)
    {
    	fprintf(stderr, "user.x failed:%s\n",strerror(errno) );
    	exit(EXIT_FAILURE);
    }
       

    value = "my attribute again..";
    if (setxattr(argv[1], "user.y", value, strlen(value), 0) == -1)
    {
    	fprintf(stderr, "user.y failed:%s\n",strerror(errno) );
    	exit(EXIT_FAILURE);
    }

    getxattr(argv[1],"user.x",buff,256);
    printf("user.x= %s\n",buff);

    getxattr(argv[1],"user.y",buff,256);
    printf("user.y= %s\n",buff);

  
     if ( removexattr(argv[1],"user.x")== -1)
    {
    	fprintf(stderr, "user.x remove failed:%s\n",strerror(errno) );
    	exit(EXIT_FAILURE);
    }
       
   
    if ( removexattr(argv[1],"user.y")== -1)
    {
    	fprintf(stderr, "user.y remove failed:%s\n",strerror(errno) );
    	exit(EXIT_FAILURE);
    }

	exit(EXIT_SUCCESS);
return 0;
}


