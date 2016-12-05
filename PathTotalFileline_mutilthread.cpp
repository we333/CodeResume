#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <map>

using namespace std;

#define MUTIL_NUM	(5)

pthread_mutex_t mutex_cnt= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_file= PTHREAD_MUTEX_INITIALIZER;
long mutli_ret = 0;
pthread_t threadNo[MUTIL_NUM];
map<char *, bool> table;

long CalcFileLines(char filename[])
{
	long ret = 0;
	string tmp;
	ifstream rd(filename, ios::in);
	while(getline(rd, tmp, '\n'))
		ret++;
	rd.close();
	return ret;
}

void *mutil(void *arg)
{
	char *dir = (char *)arg;

	pthread_mutex_lock(&mutex_file);
	if(table[dir] == false)
	{
		pthread_mutex_unlock(&mutex_file);
		table[dir] = true;
	}
	else
	{
		pthread_mutex_unlock(&mutex_file);
		return (void*)1;
	}

	DIR *pDir = opendir(dir);
	struct dirent *pDirent;

	if(NULL == pDir)
		;
	else
	{
		while(pDirent = readdir(pDir))
		{
			if(!strcmp(pDirent->d_name, ".") || !strcmp(pDirent->d_name, ".."))
				continue;

			char *name = (char *)malloc(1024);
			bzero(name, 1024);
			strcpy(name, dir);
			strcat(name, "/");
			strcat(name, pDirent->d_name);

            struct stat st;
			int f = stat(name, &st);
			if(S_ISDIR(st.st_mode))
			{
				mutil((void *)name);
			}
			else
			{
				pthread_mutex_lock(&mutex_cnt);
            	mutli_ret += CalcFileLines(name);
            	pthread_mutex_unlock(&mutex_cnt);
			}
		}
	}
	closedir(pDir);
}

long CountNumberOfTextLines( const char* dir )
{
	for(int i = 0; i < MUTIL_NUM; i++)
	{
		pthread_create(&threadNo[i],NULL,mutil,(void *)dir);
		pthread_join(threadNo[i],NULL);
	}
	return mutli_ret;
}

int main()
{
	cout<<"result = "<<CountNumberOfTextLines("/cygdrive/c/workspace/share/CodeResume");

	return 0;
}