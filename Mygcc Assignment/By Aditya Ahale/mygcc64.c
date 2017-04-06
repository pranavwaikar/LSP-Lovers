#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/types.h>

#define Asm 10
#define Obj 20
#define Exe 30

#define TRUE  0
#define FALSE 1

typedef int flag;
typedef int STAGE;

flag S_in = FALSE;
flag o_in = FALSE;		
flag c_in = FALSE;

STAGE stage = Exe;

int retval;
int err=0;
int len=0;

char *src_file = NULL;
char *dest_file = NULL;

void build(STAGE,char *,char *);

char *ASMfile(char *,char *);
char *OBJfile(char *,char *);
char *EXEfile(char *,char *);
int delete(char *);



int main (int argc,char *argv[])
{
	while((retval=getopt(argc,argv,"o:cS"))!=-1)		
	{
		switch(retval)
		{
			case 'S':
				S_in=TRUE;	
				stage=Asm;
				break;
			case 'o':
				o_in=TRUE;	
				dest_file=optarg;
				len=strlen(dest_file);
				len=len+2; 
				break;
			case 'c':
				c_in=TRUE;	
				stage=Obj;
				break;
			default:
				printf("ERROR:DEFAULT");
				break;
		}
	}
	src_file=argv[optind];
	if(o_in==FALSE)			
	{				
		len=strlen(src_file);
	}
	
	if(src_file==NULL)
	{
		fprintf(stderr,"ERROR:Source File not Specified");
		exit(EXIT_FAILURE);
	}
	else
	{
		build(stage,src_file,dest_file);
	}
	exit(EXIT_SUCCESS);
}

int del(char *filename)
{
	int derr;
	derr=unlink(filename);
	if(derr==-1)
	{
		fprintf(stdout,"Error Deleting file%s\n",filename);
		exit(EXIT_FAILURE);
		return (derr);
	}
	else
	{
		fprintf(stdout,"File Deleted:%s\n",filename);
		return 0;
	}

}



void build(STAGE stage,char *src_flie,char *dest_file)
{
	char *T_asm,*T_obj,*T_exe;	

	switch(stage)
	{
		case Asm:
			T_asm=ASMfile(src_file,dest_file);
			break;
		case Obj:
			T_asm=ASMfile(src_file,dest_file);
			T_obj=OBJfile(T_asm,dest_file);
			del(T_asm);
			break;
		case Exe:
			T_asm=ASMfile(src_file,dest_file);
			T_obj=OBJfile(T_asm,dest_file);
			T_exe=EXEfile(T_obj,dest_file);			
			del(T_asm);
			del(T_obj);
			break;
		default:
			break;
	}
}


char *ASMfile(char *src_file,char *dest_file)
{
	int asm_ret;
	char *temp=calloc(len,sizeof(char));

	
	if(dest_file==NULL)
	{
		
		strncpy(temp,src_file,len-2);
		strncat(temp,".s",len);
	}
	else	
	{
		
		strncpy(temp,dest_file,len);
		strncat(temp,".s",len);
	}


	
	if(fork()==0)
	{
		err=execlp("/usr/bin/gcc","gcc","-S","-o",temp,src_file,(char *)0);
		if(err==-1)
		{
			printf("\nERROR::exec() failed\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&asm_ret);
		fprintf(stdout,".S File Created!:%s\n",temp);
	}
	return (temp);


	
}

char *OBJfile(char *src_file,char *dest_file)
{
	int obj_ret;
	char *temp2=calloc(len,sizeof(char));
	

	
	if(dest_file==NULL)
	{
		strncpy(temp2,src_file,len-2);
		strncat(temp2,".o",len);
	}
	else	
	{
		strncpy(temp2,dest_file,len);
		strncat(temp2,".o",len);
	}
	
	
	if(fork()==0)
	{
		err=execlp("/usr/bin/as","as","-o",temp2,src_file,(char *)0);
		if(err==-1)
		{
			printf("\nERROR::exec() failed\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&obj_ret);
		fprintf(stdout,".obj File Created!:%s\n",temp2);
	}
	return (temp2); 
}

char *EXEfile(char *src_file,char *dest_file)
{
	int exe_ret;
	char *temp3=calloc(len,sizeof(char));
	
	
	
	if(dest_file==NULL)
	{
		strncpy(temp3,src_file,len-2);
		strncat(temp3,"",len);
	}
	else	
	{
		strncpy(temp3,dest_file,len);
		strncat(temp3,"",len);
	}

	
	if(fork()==0)
	{
		err=execlp("ld","ld","-o",temp3,"-lc","-dynamic-linker","/lib64/ld-linux-x86-64.so.2",src_file,"-e","main",(char *)0); // 64 bit machine
		if(err==-1)
		{
			printf("\nERROR::exec() failed\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&exe_ret);
		fprintf(stderr,"\n.exe File Created!:%s\n",temp3);
	}
	return (temp3);
}
