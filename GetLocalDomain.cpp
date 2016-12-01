#include <sys/wait.h>
#include <sys/types.h>       
#include <sys/socket.h>     
#include <sys/stat.h>       
#include <netinet/in.h>     
#include <arpa/inet.h>  
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;


bool SplitMailAddress( const char* mailAddr, string& user, string& domain )
{
	// Let's write code here !
	string email = string(mailAddr);
	if(*mailAddr == '@')
		return false;

	const char *p = mailAddr;
	while(*p != '@' && *p != '\0')
	{
		user += *p;
		p++;
	}
	
    if(*p == '\0' || *(++p) == '\0')
        return false;
	while(*p != '\0')
	{
		if(*p == '@' || *p == ' ')
			return false;
		domain += *p;
        p++;
	}
    return true;
}


int main()
{
	string user;
	string domain;
	cout<<SplitMailAddress("nigel@", user, domain);
	
	return 0;
}