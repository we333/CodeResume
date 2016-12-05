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
#include <errno.h>

using namespace std;

struct stat st;

int file_lines(char filename[])
{
	int ret = 0;
	string tmp;
	ifstream rd(filename, ios::in);
	while(getline(rd, tmp, '\n'))
		ret++;
	rd.close();
	return ret;
}

long my_ls(char dirname[])
{
	long ret = 0;
	DIR *pDir = opendir(dirname);
	struct dirent *pDirent;

	if(NULL == pDir)
		cout<<"null mkdir"<<endl;
	else
	{
		while(pDirent = readdir(pDir))
		{
			if(!strcmp(pDirent->d_name, ".") || !strcmp(pDirent->d_name, ".."))
				continue;
			ret += file_lines(pDirent->d_name);
			int f = stat(pDirent->d_name, &st);
			if(S_ISDIR(st.st_mode))
				cout<<pDirent->d_name<<" is a folder"<<endl;
			cout<<pDirent->d_name<<"-->"<<pDirent->d_type<<"-->"<<ret<<endl;
		}
	}
	closedir(pDir);
	return ret;
}

int main()
{
	cout<<my_ls(".");

//	cout<<file_lines("Calc.cpp")<<endl;;
//	cout<<all_lines<<endl;

	return 0;
}