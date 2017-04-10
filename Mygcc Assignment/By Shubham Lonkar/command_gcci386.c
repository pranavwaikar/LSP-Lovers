#include <stdio.h>

#include <getopt.h>

#include <unistd.h>

#include <sys/wait.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

#include <errno.h>

#define TRUE  0
#define FALSE 1

#define ASM 100
#define OBJ 200
#define EXE 300

typedef int flag_t;

typedef int STAGE;


flag_t asm_used = FALSE;
flag_t obj_used = FALSE;
flag_t exe_used = FALSE;

STAGE stage = EXE;

flag_t decision = TRUE;

char *src_filename = NULL;
char *custom_fname = NULL;

int ret;
int iflen = 0;
int err = 0;

int fd; 		//file descriptor for file build.log

char buff[26];

void print_help(void);

void build(STAGE ,char *,char *);

char *create_asm(char *,char *);

char *create_obj(char *,char *);

char *create_exe(char *,char *);

int file_delete(char *);

char *print_time();

flag_t decide(STAGE,char *);

int main(int argc, char *argv[])
{

	while((ret = getopt(argc,argv,"Sco:")) != -1)
	{
		switch(ret)
		{
			case 'S':
						asm_used = TRUE;
						stage = ASM;
						break;
			case 'c':
						obj_used = TRUE;
						stage = OBJ;
						break;
			case 'o':
						exe_used = TRUE;
						custom_fname = optarg;
						iflen = strlen(custom_fname);
						iflen = iflen + 2;	//for extensions			
						break;
			default:
					perror("Case Error...\n");
					print_help();
					break;			
		}
	}	

	src_filename = argv[optind];
	if (exe_used == FALSE)
	{
		iflen = strlen(src_filename);
	}

	if (src_filename == NULL)
	{
		fprintf(stderr, "\nError..Give Source file.\n");
		exit(EXIT_FAILURE);
	}
	else
		build(stage,src_filename,custom_fname);



	return 0;
}


void print_help(void){

	char help [] = "\n./command_gcc -S filename.c -> to create asm file with same filename\n\
	./command_gcc -c filename.c -> to create object file with same filename\n\
	./command_gcc -S -o  filename.c -> to create asm file with custom filename\n\
	./command_gcc -c -o filename.c -> to create object file with custom filename\n\
	./command_gcc -o filename.c  -> to create exe file with custom name\n\
	./commnad_gcc filename.c -> to create exe file named a.out\n";
	printf("%s",help);
}

void build(STAGE stage,char *src_filename,char *custom_fname)
{
char *asm_name;
char *obj_name;
char *exe_name;


fd = open("./build.log",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
if (fd == -1)
{
	perror("Failed to create log file\n");
	fprintf(stderr, "%s:%s:%s:%d\n",strerror(errno),__FILE__,__FUNCTION__,__LINE__);
	exit(EXIT_FAILURE);
}
	dup2(fd,STDOUT_FILENO);

	decision = decide(stage,src_filename);
	if (decision == FALSE)
	{
		exit(EXIT_SUCCESS);
	}

	switch(stage)
	{
		case ASM:
					asm_name = create_asm(src_filename,custom_fname);
					break;
		case OBJ:
					asm_name = create_asm(src_filename,custom_fname);
					obj_name = create_obj(asm_name,custom_fname);
					file_delete(asm_name);
					break;
		case EXE:	
					asm_name = create_asm(src_filename,custom_fname);
					obj_name = create_obj(asm_name,custom_fname);
					exe_name = create_exe(obj_name,custom_fname);
					file_delete(asm_name);
					file_delete(obj_name);
					break;			
	}

}

flag_t decide(STAGE stage,char *src_filename)
{
	int errval;
	int str_len;
	struct stat stat_buff1,stat_buff2;
	double time_diff;
	time_t t1,t2;

	str_len = strlen(src_filename);

	char *copy = calloc(str_len,sizeof(char));
	
	strncpy(copy,src_filename,str_len - 2);


	if (stage == ASM)
	{
		strncat(copy,".s",str_len);
	}

	if (stage == OBJ)
	{
		strncat(copy,".o",str_len);
	}

	if(stage == EXE)
	{
		strncat(copy,"",str_len);
	}

	fprintf(stdout, "%s:Checking previous builds:%s\n",print_time(),copy);

	errval = stat(src_filename,&stat_buff1);
	if (errval == -1)
	{
		fprintf(stdout, "%s:%s:%s\n",print_time(),src_filename,strerror(errno));
		exit(EXIT_FAILURE);
	}

	errval = stat(copy,&stat_buff2);
	if (errval == -1)
	{
		fprintf(stdout, "%s:%s:%s\n",print_time(),copy,strerror(errno));
		fprintf(stdout, "%s:No previous build found,Build necessary:%s\n",print_time(),src_filename);
		return TRUE;
	}

	t1 = stat_buff1.st_mtime;
	t2 = stat_buff2.st_mtime;

time_diff = difftime(t1,t2);

	if(time_diff > 0)
	{
		fprintf(stdout, "%s:Source file is updated,Build necessary:%s\n",print_time(),src_filename);
		return TRUE;
	}
	else
	{
		fprintf(stdout, "%s:Source file is not updated,No need to build:%s\n",print_time(),src_filename);
		return FALSE;
	}
}


char *print_time()
{
	time_t timer;
	struct  tm *tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buff,26,"%Y-%m-%d %H:%M:%S",tm_info);

	return buff;
		
}

char *create_asm(char *c_src_filename,char *asm_custom_fname)
{
	int iret;
	char *asm_temp_fname = calloc(iflen,sizeof(char));

	if(asm_custom_fname == NULL)
	{
		strncpy(asm_temp_fname,c_src_filename,iflen - 2);
		strncat(asm_temp_fname,".s",iflen);
	}
	else
	{
		strncpy(asm_temp_fname,asm_custom_fname,iflen);
		strncat(asm_temp_fname,".s",iflen);
	}

	if(fork() == 0)
	{
		err = execlp("/usr/bin/gcc","gcc","-S","-o",asm_temp_fname,c_src_filename,(char *) 0);
		if(err == -1)
		{
			printf("\nFAILED:exec:gcc\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&iret);
		fprintf(stdout, "%s:ASM file created:%s\n",print_time(),asm_temp_fname);
	}

return asm_temp_fname;	
}

char *create_obj(char *asm_src_filename,char *obj_custom_fname)
{
	int iret;

	char *obj_temp_fname = calloc(iflen,sizeof(char));

	if(obj_custom_fname == NULL)
	{
		strncpy(obj_temp_fname,asm_src_filename,iflen - 2);
		strncat(obj_temp_fname,".o",iflen);
	}
	else
	{
		strncpy(obj_temp_fname,obj_custom_fname,iflen);
		strncat(obj_temp_fname,".o",iflen);
	}

	if(fork() == 0)
	{
		err = execlp("/usr/bin/as","as","-o",obj_temp_fname,asm_src_filename,(char *)0);
		if(err == -1)
		{
			/* code */
			printf("\nFAILED:exec:as\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&iret);
		fprintf(stdout, "%s:OBJ file created:%s\n",print_time(),obj_temp_fname);		
	}

return obj_temp_fname;

}

char *create_exe(char *obj_src_filename,char *exe_custom_fname)
{
	int iret;

	char *exe_temp_fname = calloc(iflen,sizeof(char));

	if (exe_custom_fname == NULL)
	{
		exe_temp_fname = "a.out";
	}
	else
	{
		strncpy(exe_temp_fname,exe_custom_fname,iflen);
		strncat(exe_temp_fname,"",iflen);
	}
	if (fork() == 0)
	{
		err = execlp("ld","ld","-o",exe_temp_fname,"-lc","-dynamic-linker","/lib/i386-linux-gnu/ld-linux.so.2",obj_src_filename,"-e","main",(char *)0);
		if (err == -1)
		{
			fprintf(stderr, "\nFAILED:exec:ld\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&iret);
		fprintf(stdout, "%s:exe file created:%s\n",print_time(),exe_temp_fname);
	}
	return exe_temp_fname;	
}





int file_delete(char *file_name)
{
	int derr;
	derr = unlink(file_name);
	if (derr == -1)
	{
		fprintf(stdout, "%s:ERROR Deleting file:%s\n",print_time(),file_name);
		exit(EXIT_FAILURE);
		return derr;
	}
	else
	{
		fprintf(stdout, "%s:Deleting file:%s\n",print_time(),file_name);
		return 0;
	}
}