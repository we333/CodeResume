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

using namespace std;

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

long CountNumberOfTextLines( const char* dir )
{
	long ret = 0;
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

		//	string name = dir;
		//	name += "/";
		//	name += pDirent->d_name;

			char *name = (char *)malloc(1024);
			bzero(name, 1024);
			strcpy(name, dir);
			strcat(name, "/");
			strcat(name, pDirent->d_name);

            struct stat st;
			int f = stat(name, &st);
			if(S_ISDIR(st.st_mode))
				ret += CountNumberOfTextLines(name);
			else
            	ret += CalcFileLines(name);
		}
	}
	closedir(pDir);
	return ret;
}

int main()
{

	cout<<"result = "<<CountNumberOfTextLines("/cygdrive/c/workspace/share");

	return 0;
}