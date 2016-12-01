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

#define Try(x)      do{if(-1 == (x)) perror("__LINE__");}while(0);

/*
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h> 
#include <sys/socket.h>

*/
void HttpGET( const string& host, const string& path, int port, string& html )
{
    struct hostent *Host;
    Host = gethostbyname(host.c_str());
    perror("");
 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;   
    bzero((void *)&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    memcpy(&address.sin_addr, Host->h_addr, Host->h_length);

    connect(sockfd, (sockaddr *)&address, sizeof(sockaddr_in));
    perror("");

    string get_http = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    cout<<get_http<<endl;

    send(sockfd, get_http.c_str(), strlen(get_http.c_str()), 0);
    perror("");

    int rev = 0;
    char buf[4096]; bzero(buf, 4096);

    
    while(1)
    {
        rev = recv(sockfd, buf, 4096, 0);
        cout<<rev;
        html += buf;
        bzero(buf, 4096);

        if(rev == 0)
            break;
    }
}

int main()
{
    string html;
 //  string host = "www.coderesume.com";
    string host = "www.baidu.com";
    string path = "/modules/quizUtil/";
    HttpGET(host, path, 80, html);
    
    
    cout<<html<<endl;
    cout<<"----------------------------------------"<<endl;
    return 0;
}
