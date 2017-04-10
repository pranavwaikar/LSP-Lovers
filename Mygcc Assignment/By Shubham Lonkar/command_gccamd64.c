#include <stdio.h>

#include <stdlib.h>

#include <getopt.h>

#include <unistd.h>

#include <string.h>

#include <time.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <sys/wait.h>

#include <fcntl.h>

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

flag_t decision = TRUE;

STAGE stage = EXE;

char *custom_fname = NULL;
char *src_filename = NULL;
char buff[26];

int ret;
int iflen = 0;
int err = 0;
int fd;

void print_help();
void build(STAGE ,char *,char *);
char *create_asm(char *,char *);
char *create_obj(char *,char *);
char *create_exe(char *,char *);
int delete_file(char *);
flag_t decide(STAGE ,char *);
char *print_time();

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
						iflen = iflen + 2;
						break;
			default:
						perror("Case Error...\n");
						print_help();
						break;		
		}
	}
	src_filename = argv[optind];
		if(exe_used == FALSE)
			iflen = strlen(src_filename);

		if(src_filename == NULL)
		{
			fprintf(stderr, "\nError:Please give a source file\n");
			exit(EXIT_FAILURE);
		}
		else
			build(stage,src_filename,custom_fname);

	return 0;
}

void print_help()
{
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
		perror("\nError..Failed to create log file\n");
	}

	dup2(fd,STDOUT_FILENO);

	decision = decide(stage,src_filename);
	if(decision == FALSE)
	{
		exit(EXIT_FAILURE);
	}

	switch(stage)
	{
		case ASM:
				  asm_name = create_asm(src_filename,custom_fname);
				  break;
		case OBJ:
				  asm_name = create_asm(src_filename,custom_fname);
				  obj_name = create_obj(asm_name,custom_fname);
				  delete_file(asm_name);
				  break;
		case EXE:
				  asm_name = create_asm(src_filename,custom_fname);
				  obj_name = create_obj(asm_name,custom_fname);
				  exe_name = create_exe(obj_name,custom_fname);	
				  delete_file(asm_name);
				  delete_file(obj_name);
				  break;		
	}
}

char *print_time()
{
	time_t timer;
	struct tm *tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buff,26,"%Y-%m-%d %H:%M:%S",tm_info);
return buff;	
}

char *create_asm(char *src_filename,char *asm_custom_fname)
{
	int iret;

	char *asm_temp_fname = calloc(iflen,sizeof(char));

	if(asm_custom_fname == NULL)
	{
		strncpy(asm_temp_fname,src_filename,iflen - 2);
		strncat(asm_temp_fname,".s",iflen);
	}
	else
	{
		strncpy(asm_temp_fname,src_filename,iflen);
		strncat(asm_temp_fname,".s",iflen);
	}

	if(fork() == 0)
	{
		err = execlp("/usr/bin/gcc","gcc","-S","-o",asm_temp_fname,src_filename,(char *)0);
		if (err == -1)
		{
			printf("\nFailed:exec:gcc\n");
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

char *create_obj(char *asm_src_fname,char *obj_custom_fname)
{
	int iret;
	char *obj_temp_fname = calloc(iflen,sizeof(char));

	if(obj_custom_fname == NULL)
	{
		strncpy(obj_temp_fname,asm_src_fname,iflen - 2);
		strncat(obj_temp_fname,".o",iflen);
	}
	else
	{
		strncpy(obj_temp_fname,asm_src_fname,iflen);
		strncat(obj_temp_fname,".o",iflen);
	}

	if(fork() == 0)
	{
		err = execlp("/usr/bin/as","as","-o",obj_temp_fname,asm_src_fname,(char *)0);
		if (err == -1)
		{
			printf("\nFAILED:exec:as\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&iret);
		fprintf(stdout,"%s:OBJ file created:%s\n",print_time(),obj_temp_fname);
	}
	return obj_temp_fname;
}

char *create_exe(char *obj_src_fname,char *exe_custom_fname)
{
	int iret;

	char *exe_temp_fname = calloc(iflen,sizeof(char));

	if(exe_custom_fname == NULL)
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
		err = execlp("ld","ld","-o",exe_temp_fname,"-lc","-dynamic-linker","/lib64/ld-linux-x86-64.so.2",obj_src_fname,"-e","main",(char *)0);
		if(err == -1)
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

int delete_file(char *file_name)
{
	int derr;
	derr = unlink(file_name);
	if (derr == -1)
	{
		fprintf(stdout, "%s:ERROR Deleting file:%s\n",print_time(),file_name);
		exit(EXIT_FAILURE);
		return err;
	}
	else
	{
		fprintf(stdout, "%s:Deleting file:%s\n",print_time(),file_name);
		return 0;
	}
}

flag_t decide(STAGE stage,char *src_filename)
{
	int errval;
	struct stat stat_buff1,stat_buff2;
	double time_diff;
	time_t t1,t2;

	int str_len;

	str_len = strlen(src_filename);

	char *copy = calloc(str_len,sizeof(char));
	strncpy(copy,src_filename,str_len - 2);

	if(stage == ASM)
	{
		strncat(copy,".s",str_len);
	}
	if(stage == OBJ)
	{
		strncat(copy,".o",str_len);
	}
	if(stage == EXE)
	{
		strncat(copy,"",str_len);
	}

	fprintf(stdout, "%s:Checking previous builds:%s\n",print_time(),copy);

	errval = stat(src_filename,&stat_buff1);
	if(errval == -1)
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

	if (time_diff > 0)
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