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

//char lc[] = {'!','#','$','%', '&', '\'', '*', '+', '-', '/', '=', '^', '_', '{', '}', '|', '~'};
char lc[] = {"!#$%&'*+-/=?^_`{|}.~"};
bool isA(char p){
    if(p >= 'A' && p <= 'Z')    return true;
    if(p >= 'a' && p <= 'z')    return true;
    return false;
}
bool isN(char p){
    if(p >= '0' && p <= '9')    return true;
    return false;
}
bool ChkL(char p){
    if(isA(p) || isN(p))   return true;
    for(int i = 0; i < 21; i++)
        if(p == lc[i])  return true;
    return false;
}
bool ChkD(char p){
    if(isA(p) || isN(p) || p == '-')   return true;
    return false;
}
bool IsValidMailAddressFormat( const char* p ){
    bool lp,dp,at;
    int i = 1, lLen = 0,dLen = 0;
    string e = string(p);   
    int eLen = e.size();
    if(eLen < 5 || e[0] == '.') goto F;
    for(; i < eLen; i++){
        if(i > 64)  goto F;
        else if(e[i] == '@'){
            if(at)  goto F;
            at = true;
            if(e[i-1] == '.') goto F;
            break;
        }else if(e[i] == '.'){
            if(lp)  goto F;
            lp = true;
        }else{
            lp = false;
            if(!ChkL(e[i])) goto F;
        }
    }
    if(!at) goto F;
    lLen = i;
    if(i == eLen-1 || !isA(e[i+1])) goto F;
    i++;
    for(; i <eLen; i++){
        dLen++;
        if(dLen > 255 || e[i] == '@') goto F;
        else if(e[i] == '.'){
            if(dp)  goto F;
            dp = true;
        }else
            if(!ChkD(e[i])) goto F;
    }
    if(dLen < 3 || !isA(e[--i])) goto F;
    return true;
    F: return false;
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
