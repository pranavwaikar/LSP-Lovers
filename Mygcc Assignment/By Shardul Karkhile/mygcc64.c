#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<getopt.h>
#include<sys/wait.h>
#include<sys/types.h>

#define ASSEMBLYFILE 1000
#define OBJECTFILE 2000
#define EXECUTABLEFILE 3000

typedef int FLAG;
typedef int STAGE;

FLAG Switch_S=0;
FLAG Switch_o=0;
FLAG Switch_c=0;

STAGE Stage=EXECUTABLEFILE;

int ReturnValue,Error=0,Length=0;

char *SourceFileName=NULL;
char *DestinationFileName=NULL;


void BuildingProcedure(STAGE,char *,char *);

char *CreatingAssemblyFile(char *,char *);
char *CreatingObjectFile(char *,char *);
char *CreatingExecutableFile(char *,char *);
int DeletingFile(char *);

int main(int argc,char *argv[])
{
	while((ReturnValue=getopt(argc,argv,"co:S"))!=-1)
	{
		switch(ReturnValue)
		{	
			case 'S':
				printf("\nSwitch S used Sucessfully!!!\n");
				Switch_S=1;
				Stage=ASSEMBLYFILE;
				break;
			case 'o':
				Switch_o=1;
				DestinationFileName=optarg;
				Length=strlen(DestinationFileName);
				break;
			case 'c':
				Switch_c=1;
				Stage=OBJECTFILE;
				break;
			default:
				printf("\nSwitch Case Default ERROR!!!!\n");
				break;
		}
	}
	SourceFileName=argv[optind];
	if(SourceFileName==NULL)
	{
		fprintf(stderr,"\nSource file Name Not Given?? !!!!ERROR!!!!!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		BuildingProcedure(Stage,SourceFileName,DestinationFileName);
	}
	exit(EXIT_SUCCESS);
}

int DeletingFile(char *FileName)
{
	int DError;
	DError=unlink(FileName);
	if(DError==-1)
	{
		fprintf(stdout,"Deleting File ERROR!!!!!%s\n",FileName);
		return (DError);
	}
	else
	{
		fprintf(stdout,"File Deleted Sucessfully!!!%s\n",FileName);
		return 0;
	}
}

char *CreatingAssemblyFile(char *SourceFileName,char *DestinationFileName)
{
	int AReturn;
	char *TemporaryFileName=calloc(Length,sizeof(char));
	strncpy(TemporaryFileName,SourceFileName,Length);
	
	if(DestinationFileName==NULL)
	{
		TemporaryFileName="a.out.S";
	}
	else 
	{
		strncat(TemporaryFileName,".s",Length);
	}
	if(fork()==0)
	{
		Error=execlp("/usr/bin/gcc","gcc","-S","-o",TemporaryFileName,SourceFileName,(char *)0);
		if(Error==-1)
			printf("\nFunction exec() failed Sucessfully....ERROR(gcc?)!!!!\n");
	}
	else
	{
		wait(&AReturn);
		fprintf(stdout,".S File Created Sucessfully:--%s\n",TemporaryFileName);
	}
	return (TemporaryFileName);
}

char *CreatingObjectFile(char *SourceFileName,char *DestinationFileName)
{
	int OReturn;
	char *TemporaryFileName=calloc(Length,sizeof(char));
	strncpy(TemporaryFileName,SourceFileName,Length);
	
	if(DestinationFileName==NULL)
	{
		TemporaryFileName="a.out.o";
	}
	else
	{
		strncat(TemporaryFileName,".o",Length);
	}
	if(fork()==0)
	{
		Error=execlp("/usr/bin/as","as","-o",TemporaryFileName,SourceFileName,(char *)0);
		if(Error==-1)
			printf("\nFunction exec() failed Sucessfully....ERROR(as?)!!!\n");
	}
	else
	{
		wait(&OReturn);
		fprintf(stdout,"Object File Created Sucesfully:%s\n",TemporaryFileName);
	}
	return (TemporaryFileName);
}

char *CreatingExecutableFile(char *SourceFileName,char *DestinationFileName)
{
	int EReturn;
	char *TemporaryFileName=calloc(Length,sizeof(char));
	strncpy(TemporaryFileName,SourceFileName,Length);
	
	if(DestinationFileName==NULL)
	{	
		TemporaryFileName="a.out";
	}
	else
	{
		strncat(TemporaryFileName,".exe",Length);
	}
	if((fork()==0))
	{
		Error=execlp("ld","ld","-o",TemporaryFileName,"-lc","-dynamic-linker","/lib64/ld-linux-x86-64.so.2",SourceFileName,"-e","main",(char *)0);

		if(Error==-1)
			fprintf(stderr,"\nFunction exec() failed Sucessfully...ERROR(ld?)!!!\n");
	}
	else
	{
		wait(&EReturn);
		fprintf(stderr,"\n.exe File Created Sucessfully:--%s\n",TemporaryFileName);
	}
	return (TemporaryFileName);
}

void BuildingProcedure(STAGE Stage,char *SourceFileName,char *DestinationFileName)
{
	char *TemporaryAssemblyFile,*TemporaryObjectFile,*TemporaryExecutableFile;
	switch(Stage)
	{
		case ASSEMBLYFILE:
			TemporaryAssemblyFile=CreatingAssemblyFile(SourceFileName,DestinationFileName);
			break;
		case OBJECTFILE:
			TemporaryAssemblyFile=CreatingAssemblyFile(SourceFileName,DestinationFileName);
			TemporaryObjectFile=CreatingObjectFile(TemporaryAssemblyFile,DestinationFileName);
			DeletingFile(TemporaryAssemblyFile);
			break;
		case EXECUTABLEFILE:
			TemporaryAssemblyFile=CreatingAssemblyFile(SourceFileName,DestinationFileName);
			TemporaryObjectFile=CreatingObjectFile(TemporaryAssemblyFile,DestinationFileName);
			TemporaryExecutableFile=CreatingExecutableFile(TemporaryObjectFile,DestinationFileName);
			DeletingFile(TemporaryAssemblyFile);
			DeletingFile(TemporaryObjectFile);
			break;
		default:
			break;
	}
}
