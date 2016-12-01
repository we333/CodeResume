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

const char localChar[] = {'!', '#', '$', '%', '&', '\'', '*', '+', '-', '/', '=', '^', '_', '{', '}', '|', '~'};

bool isAlphabet(char p)
{
	if(p >= 'A' && p <= 'Z')	return true;
	if(p >= 'a' && p <= 'z')	return true;
	return false;
}

bool isNumber(char p)
{
	if(p >= '0' && p <= '9')	return true;
	return false;
}

bool IsValidLocal(char p)
{
	if(isAlphabet(p))	return true;
	if(isNumber(p))		return true;
	for(int i = 0; i < sizeof(localChar)/sizeof(localChar[0]); i++)
		if(p == localChar[i])
			return true;
	return false;
}

bool isValidDomain(char p)
{
	if(isAlphabet(p))	return true;
	if(isNumber(p))		return true;
	if(p == '-')	return true;
	return false;
}

bool IsValidMailAddressFormat( char* p )
{
	bool prevIsPoint = false;
	bool domain_point = false;
	bool appear_at = false;
	int local_len = 0;
	int domain_len = 0;
	string email = string(p);	

	int email_size = email.size();
	if(email_size < 5)
	{
cout<<"err0 	";	
		return false;
	}
	if(email[0] == '.')
	{
cout<<"err1 	";	// .开头				
		return false;
	}

	int i = 1;
	for(; i < email_size; i++)
	{
		if(i > 64)
		{		
cout<<"err2 	";		// local超过64			
			return false;
		}
		else if(email[i] == '@')
		{// local over
			if(appear_at)						
			{
cout<<"err3 	";	
				return false;
			}
			appear_at = true;

			if(email[i-1] == '.')				
			{
cout<<"err4 	";	// .@
				return false;
			}
			break;
		}
		else if(email[i] == '.')
		{
			if(prevIsPoint)
			{
cout<<"err5 	";	// ...
				return false;
			}
			prevIsPoint = true;
		}
		else
		{
			prevIsPoint = false;
			if(!IsValidLocal(email[i]))			
			{
cout<<"err6 	";	// 空格
				return false;
			}
		}
	}
	if(!appear_at)		
	{
cout<<"err7 	";	// 没有出现@	
		return false;
	}
	local_len = i;

	if(i == email_size-1)				
	{
cout<<"err8 	";	
		return false;
	}
	if(!isAlphabet(email[i+1]))		
	{
cout<<"err9 	";	// @.
		return false;
	}

	i++;
	for(; i <email_size; i++)
	{
		domain_len++;
		if(domain_len > 255)			
		{
cout<<"err10	";	
			return false;
		}
		if(email[i] == '@')				
		{
cout<<"err11	";		//domain出现了@
			return false;
		}
		else if(email[i] == '.')
		{
			if(domain_point)
			{
cout<<"err12	";	// domain多个.
				return false;
			}
			domain_point = true;
		}
		else
		{
			if(!isValidDomain(email[i]))
			{
cout<<"err13	";	// 问号
				return false;
			}
		}
	}
	if(domain_len < 3)	
	{
cout<<"err14	";	
		return false;
	}
	if(!isAlphabet(email[--i]))	
	{
cout<<"err15	";	// .结尾
		return false;
	}

	return true;
}


int main ()
{
	char s1[] = "michael@coderesume.com";
	char s2[] = "michaelcoderesume.com";
	char s3[] = "@coderesume.com";
	char s4[] = "@";
	char s5[] = "michael@schumacher@coderesume.com";
	char s6[] = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890@coderesume.com";
	char s7[] = ".ayrton@coderesume.com";
	char s8[] = "ayrton.@coderesume.com";
	char s9[] = "ayrton.senna...da.silva@coderesume.com";
	char s10[] = "fernando alonso@coderesume.com";
	char s11[] = "kimi@xxxxxxxxxx.the.length.of.this.domain.is.256.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	char s12[] = "kimi@coderesume.";
	char s13[] = "kimi@.coderesume.com";
	char s14[] = "kimi@coderesume..com";
	char s15[] = "kimi@code?resume.com";

	cout<<"s1 = "<<IsValidMailAddressFormat(s1)<<endl;
	cout<<"s2 = "<<IsValidMailAddressFormat(s2)<<endl;
	cout<<"s3 = "<<IsValidMailAddressFormat(s3)<<endl;
	cout<<"s4 = "<<IsValidMailAddressFormat(s4)<<endl;
	cout<<"s5 = "<<IsValidMailAddressFormat(s5)<<endl;
	cout<<"s6 = "<<IsValidMailAddressFormat(s6)<<endl;
	cout<<"s7 = "<<IsValidMailAddressFormat(s7)<<endl;
	cout<<"s8 = "<<IsValidMailAddressFormat(s8)<<endl;
	cout<<"s9 = "<<IsValidMailAddressFormat(s9)<<endl;
	cout<<"s10 = "<<IsValidMailAddressFormat(s10)<<endl;
	cout<<"s11 = "<<IsValidMailAddressFormat(s11)<<endl;
	cout<<"s12 = "<<IsValidMailAddressFormat(s12)<<endl;
	cout<<"s13 = "<<IsValidMailAddressFormat(s13)<<endl;
	cout<<"s14 = "<<IsValidMailAddressFormat(s14)<<endl;
	cout<<"s15 = "<<IsValidMailAddressFormat(s15)<<endl;
/**/
	return 0;
}
