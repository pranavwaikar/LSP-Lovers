#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>

#define ASSEMBLY_FILE	10
#define OBJECT_FILE	20
#define EXECUTABLE_FILE	30

#define TRUE	0
#define FALSE	1

typedef int flag_t;
typedef int STAGE;

flag_t s_switch = FALSE;	// -s switch flag
flag_t o_switch = FALSE;	// -o switch flag
flag_t c_switch = FALSE;	// -c switch flag

STAGE stage = EXECUTABLE_FILE;

flag_t decision = TRUE;

int returnValue;	//return value from getopt file
int error = 0;		// errno
int length = 0;		//length of custom name string
int fdBuild;		//file descriptor for build.log file

char *srcFileName  = NULL;	//pointer to source file name
char *destFileName = NULL;	//pointer to destination file name
char buffer[26];

void buildProcedure(STAGE, char *, char *);	//stage, sourceFileName, destFileName
char *createAssemblyFile(char *, char *);	//source file name(.c file), destination file name (.s file)
char *createObjectFile(char *, char *);		//source file name(.s file), destination file name (.o file)
char *createExecutableFile(char *, char *);	//source file name(.o file), destination file name (executable file)
char *print_time();				//returns date and time
int deleteFile(char *);				//delete file
void systemError(char *);				//for error
flag_t decide(STAGE, char *);			//build the file again or skip

int main(int argc, char *argv[])
{
	while ((returnValue = getopt(argc, argv, "Sco:")) != -1)
	{
		switch (returnValue)
		{
			case 'S':
				s_switch = TRUE;
				stage = ASSEMBLY_FILE;
				break;
				
			case 'o':
				o_switch = TRUE;
				destFileName = optarg;
				length = strlen(destFileName);
				length = length + 2; //for extension
				break;
				
			case 'c':
				c_switch = TRUE;
				stage = OBJECT_FILE;
				break;
				
			default:
				printf("\nInvalid switch.\n");
				break;
		}
	}
	
	srcFileName = argv[optind];
	
	if (o_switch == FALSE)
	{
		length = strlen(srcFileName);
	}
	
	if (srcFileName == NULL)
	{
		fprintf(stderr, "\nerror : please provide a source filename.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		buildProcedure(stage, srcFileName, destFileName);
	}
	
	exit(EXIT_SUCCESS);
	return 0;
	
}

void buildProcedure(STAGE stage, char *srcFileName, char *destFileName)
{
	char *objFileName, *exeFileName, *asmFileName;
	
	//create new log file
	fdBuild = open("./build.log", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP);
	
	if (fdBuild == -1)
	{
		systemError("failed to create log file");	
	}

	dup2(fdBuild, STDOUT_FILENO);	//redirect stdout to build.log file
	
	//chaeck for previous builds
	decision = decide(stage, srcFileName);
	
	if (decision == FALSE)
	{
		exit(EXIT_SUCCESS);
	}
	
	switch (stage)
	{
		case ASSEMBLY_FILE:
			asmFileName = createAssemblyFile(srcFileName, destFileName);
			break;
			
		case OBJECT_FILE:
			asmFileName = createAssemblyFile(srcFileName, destFileName);
			objFileName = createObjectFile(asmFileName, destFileName);
			deleteFile(asmFileName);
			break;
			
		case EXECUTABLE_FILE:
			asmFileName = createAssemblyFile(srcFileName, destFileName);
			objFileName = createObjectFile(asmFileName, destFileName);
			exeFileName = createExecutableFile(objFileName, destFileName);
			deleteFile(asmFileName);
			deleteFile(objFileName);
			break;
	}
}

void systemError(char *str)
{
	fprintf(stderr, "%s : %s", str, strerror(errno));
	exit(EXIT_FAILURE);
}

int deleteFile(char *fileName)
{
	int error;
	error = unlink(fileName);
	
	if (error == -1)
	{
		fprintf(stdout, "%s : error deleting file %s\n", print_time(), fileName);
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(stdout, "%s : deleting file %s\n", print_time(), fileName);
		return 0;
	}
}

char *print_time()
{
	time_t timer;
	struct tm *tm_info;
	
	time(&timer);
	
	tm_info = localtime(&timer);
	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	return buffer;
}

char *createAssemblyFile(char *srcFileName, char *asmFileName)
{
	int returnValueAsm;
	
	//copy string into local variable
	
	char *tempAsmFile = calloc(length, sizeof(char));
	
	if (asmFileName == NULL)
	{
		strncpy(tempAsmFile, srcFileName, length - 2); 	//length - 2 to cut the file extension i.e .c
		strncat(tempAsmFile, ".s", length);		//give .s file extension
	}
	else
	{
		//if it is obj file
		strncpy(tempAsmFile, asmFileName, length);
		strncat(tempAsmFile, ".s", length);
	}
	
	if (fork() == 0)
	{
		error = execlp("/usr/bin/gcc", "gcc", "-S", "-o", tempAsmFile, srcFileName, (char *)0);
		if (error == -1)
		{
			printf("\nfailed : exec : gcc\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&returnValueAsm);
		fprintf(stdout, "%s : assembly file generated : %s\n", print_time(), tempAsmFile);
	}
	
	return tempAsmFile;
}

char *createObjectFile(char *srcFileName, char *objFileName)
{
	int returnValueObj;
	
	char *tempObjFile = calloc(length, sizeof(char));
	
	if (objFileName == NULL)
	{
		strncpy(tempObjFile, srcFileName, length - 2);
		strncat(tempObjFile, ".o", length);
	}
	else
	{
		strncpy(tempObjFile, objFileName, length);
		strncat(tempObjFile, ".o", length);
	}
	
	if (fork() == 0)
	{
		error = execlp("/usr/bin/as", "as", "-o", tempObjFile, srcFileName, (char *)0);
		if (error == -1)
		{
			printf("\nfailed : exec : as\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&returnValueObj);
		fprintf(stdout, "%s : OBJ file created %s\n", print_time(), tempObjFile);
	}
	return tempObjFile;
}

char *createExecutableFile(char *objFileName, char *exeFileName)
{
	int returnValueExe;
	
	char *tempExeFile = calloc(length, sizeof(char));
	
	if (exeFileName == NULL)
	{
		tempExeFile = "a.out";
	}
	else
	{
		strncpy(tempExeFile, exeFileName, length);
		strncat(tempExeFile, "",length);
	}
	
	if (fork() == 0)
	{
		error = execlp("ld", "ld", "-o", tempExeFile, "-lc", "-dynamic-linker", "/lib64/ld-linux-x86-64.so.2", objFileName, "-e", "main", (char *)0);
		
		if (error == -1)
		{
			fprintf(stderr, "\nfailed : exec : ld\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&returnValueExe);
		fprintf(stdout, "%s : exe file created %s\n", print_time(), tempExeFile);
	}
	
	return tempExeFile;
}

flag_t decide(STAGE stage, char *srcFileName)
{
	int errValue;
	double timeDifference;
	struct stat statBuffer1, statBuffer2;
	time_t t1, t2;
	
	//create a copy to perform string operations
	int stringLength;
	stringLength = strlen(srcFileName);
	
	char *copy = calloc(stringLength, sizeof(char));
	strncpy(copy, srcFileName, stringLength - 2);
	
	if (stage == ASSEMBLY_FILE)
	{
		strncat(copy, ".s", stringLength);
	}
	
	if (stage == OBJECT_FILE)
	{
		strncat(copy, ".o", stringLength);
	}
	
	if (stage == EXECUTABLE_FILE)
	{
		strncat(copy, "", stringLength);
	}
	
	fprintf(stdout, "%s : looking for previous builds : %s\n", print_time(), copy);
	
	errValue = stat(srcFileName, &statBuffer1);
	
	if (errValue == -1)
	{
		fprintf(stdout, "%s : %s : %s\n", print_time(), srcFileName, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	errValue = stat(copy, &statBuffer2);
	
	if (errValue == -1)
	{
		fprintf(stdout, "%s : %s : %s\n", print_time(), copy, strerror(errno));
		fprintf(stdout, "%s : no previous build found : %s\n", print_time(), srcFileName);
		return TRUE;
	}
	
	t1 = statBuffer1.st_mtime;
	t2 = statBuffer2.st_mtime;
	
	timeDifference = difftime(t1, t1); //compare .c file for last modification time stamp
	
	if (timeDifference > 0) // if > 0, then file is updated
	{
		fprintf(stdout, "%s : source file is updated : %s\n", print_time(), srcFileName);
		return TRUE;
	}
	else //file is not updated
	{
		fprintf(stdout, "%s : source file is not updated : %s\n", print_time(), srcFileName);
		return FALSE;
	}
}

