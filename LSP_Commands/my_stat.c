#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
//std unix libs
#include<unistd.h>
#include<stdlib.h>
//error handeling 
#include<errno.h>
#include<string.h>
#include<getopt.h>
#include<time.h>


#define TRUE  0
#define FALSE 1


//flags for required fields
/*
What is this?
We are creating a unique number for used flags combination like permission model of unix.
These are flags,when set: a perticular bit number will be set;
eg. if DEV_ID is used the zeroth bit will be set (in binary-- 0000 0000 0001)
eg. if STAT_CHG_TIME is used the 12th bit be set  (in binary--1000 0000 0000)
*/

#define DEV_ID           1	//st_dev
#define INODE_NO         2	//st_ino
#define FILE_TYPE        4 	//st_mode
#define NLINK	         8	//st_nlink    no of hardlinks
#define UID             16	//st_uid
#define GID             32	//st_gid
#define SIZE            64	//st_size     file size
#define BLK_SIZE       128      //st_blksize  filesystem block size
#define REQ_BLOCKS     256      //st_blocks   no of blocks allocated
#define ACC_TIME       512      //st_atime    last  access time
#define MOD_TIME      1024      //st_mtime    last modification time
#define STAT_CHG_TIME 2048      //st_ctime last status change


typedef unsigned int flag_t;


flag_t given_flags=0; 		//store userinput flag
flag_t flag_used=FALSE;		//for checking if any custom flag is used or not
flag_t h_used=FALSE;		//help used
flag_t v_used=FALSE;		//version used


//for long options
static struct option opts_arr[]=
{
	{"help",no_argument,0,'h'},
	{"version",no_argument,0,'v'},
	{"devid",no_argument,0,'d'},
	{"inode",no_argument,0,'i'},
	{"filetype",no_argument,0,'f'},
	{"links",no_argument,0,'l'},
	{"uid",no_argument,0,'u'},
	{"gid",no_argument,0,'g'},
	{"size",no_argument,0,'s'},
	{"blocksize",no_argument,0,'b'},
	{"reqblocks",no_argument,0,'r'},
	{"acctime",no_argument,0,'a'},
	{"modtime",no_argument,0,'m'},
	{"statchgtime",no_argument,0,'c'},
	{0,0,0,0}
};

//for stat system call ret val buffer
static struct stat sb;

char *src_file_name=NULL;


int ret; 	//for getopt ret value
int i=0;	//index of getopt

int err;	//syscall ret val checking


void print_help();
void print_version();
void my_stat(char *,flag_t);		//stat(),check bits used
void display(int);			//print the info

int main(int argc,char *argv[])
{
	while((ret=getopt_long(argc,argv,"hvdiflugsbramc",opts_arr,&i))!=-1)
	{
		switch(ret)
		{
		case 'h':
			h_used=TRUE;
			print_help();
			break;
		case 'v':
			v_used=TRUE;
			print_version();
			break;
		case 'd':
			given_flags = given_flags|DEV_ID;
			flag_used=TRUE;
			break;
		case 'i':
			given_flags = given_flags|INODE_NO;
			flag_used=TRUE;
			break;
		case 'f':
			given_flags = given_flags|FILE_TYPE;
			flag_used=TRUE;
			break;	
		case 'l':
			given_flags = given_flags|NLINK;
			flag_used=TRUE;
			break;
		case 'u':
			given_flags = given_flags|UID;
			flag_used=TRUE;
			break;
		case 'g':
			given_flags = given_flags|GID;
			flag_used=TRUE;
			break;
		case 's':
			given_flags = given_flags|SIZE;
			flag_used=TRUE;
			break;
		case 'b':
			given_flags = given_flags|BLK_SIZE;
			flag_used=TRUE;
			break;
		case 'r':
			given_flags = given_flags|REQ_BLOCKS;
			flag_used=TRUE;
			break;
		case 'a':
			given_flags = given_flags|ACC_TIME;
			flag_used=TRUE;
			break;
		case 'm':
			given_flags = given_flags|MOD_TIME;
			flag_used=TRUE;
			break;
		case 'c':
			given_flags = given_flags|STAT_CHG_TIME;
			flag_used=TRUE;
			break;
		}
	}

	
	//getting source file name which is last in command.
	src_file_name=argv[optind];
	
	//if the options are only help or version,no other flags are used,file name not given thenwe will terminate
	if ( (h_used==TRUE || v_used==TRUE) && (flag_used==FALSE) && (src_file_name==NULL)) 
	{
		exit(EXIT_SUCCESS);
	}
	
	//for default behaviour
	if(flag_used==FALSE)
	{ 
		//if nothing specific is given, we will print all the info. so giving all the flags
		given_flags=given_flags|DEV_ID|INODE_NO|FILE_TYPE|NLINK|UID|GID|SIZE|BLK_SIZE|REQ_BLOCKS|ACC_TIME|MOD_TIME|STAT_CHG_TIME;
	}
	
	if(src_file_name==NULL)
	{
		fprintf(stderr,"\nFatal:Error:missing input file\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		my_stat(src_file_name,given_flags);
	}
return 0;
exit(EXIT_SUCCESS);
}


void my_stat(char *src_file_name,flag_t given_flags)
{
int position;
int result;
	err=stat(src_file_name,&sb);
	if(err==-1)
	{
		fprintf(stderr,"\nStat failed:src_file=%s:%s:Error:%s:%s:%d,\n",src_file_name,strerror(errno),__FILE__,__FUNCTION__,__LINE__);
		exit(EXIT_FAILURE);
	}

//testing the bit and if set then printing the result
	for(position=0;position<12;position++)
	{
		result=(given_flags >> (position));
		
		if(result & 1)//if bit is set to 1
		{
			display(position);	
		}	
	}
	
}


void display(int position)
{
/*
The position represent the bits which are set or unset.
according to that we are printing results here.
*/
	switch(position)
	{
	case 0: //st_dev
		fprintf(stdout,"\nDevice ID=%ld\n",(long)sb.st_dev);
		break;
	case 1://st_ino
		fprintf(stdout,"\nInode number=%ld\n",(long)sb.st_ino);
		break;
	case 2://st_mode ie file-type
		switch(sb.st_mode & S_IFMT) 				//applying bit mask for the file type bit fields
		{
		case S_IFBLK:
			fprintf(stdout,"\nFile type=Block device\n");
			break;
		case S_IFCHR:
			fprintf(stdout,"\nFile type=Character device\n");
			break;
		case S_IFDIR:
			fprintf(stdout,"\nFile type=Directory\n");
			break;
		case S_IFIFO:
			fprintf(stdout,"\nFile type=FIFO \n");
			break;
		case S_IFLNK:
			fprintf(stdout,"\nFile type=Symbolic link\n");
			break;
		case S_IFREG:
			fprintf(stdout,"\nFile type=Regular\n");
			break;
		case S_IFSOCK:
			fprintf(stdout,"\nFile type=Socket\n");
			break;
		default:
			fprintf(stdout,"\nFile type=Unknown\n");
			break;	
		}
		break;
	case 3://st_nlink
		fprintf(stdout,"\nNumber of hard links=%ld\n",(long)sb.st_nlink);
		break;
	case 4://st_uid
		fprintf(stdout,"\nUser ID=%ld\n",(long)sb.st_uid);
		break;
	case 5://st_gid
		fprintf(stdout,"\nGroup ID=%ld\n",(long)sb.st_gid);
		break;
	case 6://st_size
		fprintf(stdout,"\nFile size in bytes=%lld\n",(long long)sb.st_size);
		break;
	case 7://st_blksize
		fprintf(stdout,"\nBlock size of file system=%ld\n",(long)sb.st_blksize);
		break;
	case 8://st_blocks
		fprintf(stdout,"\nNumber of allocated blocks=%ld\n",(long)sb.st_dev);
		break;
	case 9://st_atime
		fprintf(stdout,"\nLast access time=%s\n",ctime(&sb.st_atime));
		break;
	case 10://st_mtime
		fprintf(stdout,"\nLast modification time=%s\n",ctime(&sb.st_mtime));
		break;
	case 11://st_ctime
		fprintf(stdout,"\nLast status change time=%s\n",ctime(&sb.st_atime));
		break;
	
	}
}


void print_help()
{
	static char buff[]="\nThis is help. Stat options:\n\
	1)--version       or -v\n\
	2)--devid         or -d \n\
	3)--inode         or -i\n\
	4)--filetype      or -f\n\
	5)--links         or -l\n\
	6)--uid           or -u\n\
	7)--gid           or -g\n\
	8)--size          or -s\n\
	9)--blocksize     or -b\n\
	10)--reqblocks    or -r\n\
	11)--acctime      or -a\n\
	12)--modtime      or -m\n\
	13)--statchgtime  or -c\n\
	Thank you!\
	\n";
	printf("\n%s",buff);

}


void print_version()
{
	printf("\nThis is version 0.1\n");
}

