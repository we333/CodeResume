#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

void print_tree(const char *filename,int blank);

int main(int argc, char *argv[])
{
	printf("/home/p/p/SigKill\n");
	print_tree("/cygdrive/c/workspace/share/src",2);
	return 0;
}

void print_tree(const char *filename,int blank){

	DIR *dir = opendir(filename);
	if(dir == NULL)
		perror("opendir");

 	struct dirent *ent;
	int i=0;
 
 	while((ent=readdir(dir)) != NULL)
 	{
 
 		if(!strcmp(ent->d_name,".") ||\
 			!strncmp(ent->d_name,"..",2))
 		continue;
 
	 	struct stat st;
	 
	 	char *name = (char *)malloc(sizeof(char)*1024);
	 	memset(name,'\0',sizeof(char)*1024);
	 	strcpy(name,filename);
	 	strcat(name,"/");
	 	strcat(name,ent->d_name);

	 	int f=stat(name,&st);
	 	if(f == -1){
	 		switch(errno){
	 			case EACCES:puts("EACCES");break;
	 			case EBADF:puts("EBADF");break;
	 			case EFAULT:puts("EFAULT");break;
	 			case ENOENT:puts("ENOENT");break;
	 			case ENOMEM:puts("ENOMEM");break;
	 			case ENOTDIR:puts("ENOTDIR");break;
	 		}
	 	}
 
	 	if(S_ISDIR(st.st_mode))
	 	{
	 		for(i=0;i<blank;i++)
	 			printf(" ");
	 		char *name = (char *)malloc(sizeof(char)*1024);
	 		memset(name,'\0',sizeof(char)*1024);
	 		strcpy(name,filename);
	 		strcat(name,"/");
	 		strcat(name,ent->d_name);
	 		printf("%s\n",ent->d_name);
	 		print_tree(name,blank+2);
	 		free(name);
	 	}
	 	else
	 	{
	 		for(i=0;i<blank;i++)
	 			printf(" ");
	 		printf("%s\n",ent->d_name);
	 	}
 	}
 
 }