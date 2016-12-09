/*
	1.单个线程统计完目录下所有文件名,并存入queue
	2.多个线程同时从queue中取出文件名(此操作加锁),并行计算各个文件的行数
	3.每次计算完一个文件的行数,就将其结果统计入总行数中(此操作加锁)
	!!create和join需要分开执行,否者[0]线程的join动作会阻塞后续线程启动
*/

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
#include <queue>

using namespace std;

#define MUTIL_NUM	(5)

pthread_mutex_t mutex_file= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_ret= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_thread= PTHREAD_MUTEX_INITIALIZER;
long mutli_ret = 0;
pthread_t threadNo[MUTIL_NUM];
queue<char *> fileList;
int livingThread = MUTIL_NUM;

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

void *worker(void *arg)
{
	cout<<"ret = "<<mutli_ret<<endl;	// 判断是否多个线程同时进入了入口函数

	while(1)
	{
		char *file;
		pthread_mutex_lock(&mutex_file);
		if(fileList.empty())
		{
			pthread_mutex_unlock(&mutex_file);
			return (void*)0;
		}
		file = fileList.front();
		fileList.pop();
		pthread_mutex_unlock(&mutex_file);

		long ret = CalcFileLines(file);

		pthread_mutex_lock(&mutex_ret);
		mutli_ret += ret;
		pthread_mutex_unlock(&mutex_ret);
	}

	return (void*)0;
}

void SaveTotalFile(const char* dir)
{
	DIR *pDir = opendir(dir);
	struct dirent *pDirent;

	if(NULL == pDir)
		return;

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
			SaveTotalFile(name);
		else
        	fileList.push(name);
	}
	
	closedir(pDir);
}

long CountNumberOfTextLines( const char* dir )
{
	SaveTotalFile(dir);

	for(int i = 0; i < MUTIL_NUM; i++)
		pthread_create(&threadNo[i],NULL,worker,(void *)dir);	
	for(int i = 0; i < MUTIL_NUM; i++)
		pthread_join(threadNo[i], NULL);

	return mutli_ret;
}

int main()
{
	cout<<"result = "<<CountNumberOfTextLines("/cygdrive/c/workspace/share/muduo");

	return 0;
}