#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

//type in terminal:
//ifconfig
//look up your IP address and write it below
//#define IP_ADDRESS "127.0.1.1"//"192.168.1.4"
#define PORT 8080
//1024 < PORT number < 65000

char header[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";
int head = sizeof(header);


//wraper for easier readable code check-up
void error(char* message)
{
    perror(message);
    exit(1);
}


int main(int argc, char *argv[])
{
    if(argv[1] == NULL)
    {
    printf("please provide a terminal argument\n");
    printf("./server /apsolut/path/index.html\n");
    printf("index.html is the name of the .html file\n");
    printf("if ./server and index.html are in the same directory\n");
    printf("it is sufficient to call:\n");
    printf("./server index.html\n");
    }

   FILE *fp = fopen(argv[1], "r");
    if(fp == NULL)
        error("fopen");
           
    fseek(fp, 0L, SEEK_END);
    long int sz = ftell(fp); //get the size of the .html file
    fseek(fp, 0L, SEEK_SET);
    
    int counter = 0;
    char c;
    
    while(1)
    {
        c = fgetc(fp);
        if(c == '\n') //count the number of \n
            counter++; //instead of \n we have to end each line with \r\n
        if(feof(fp)) 
            break;
    }

    fseek(fp, 0L, SEEK_SET); //the whole .html file will be the size of
    char webpage[head + sz + counter]; //header + .html file size + number of \n
    strncpy(webpage, header, head); //putting the header into webpage
    
    while(1) 
    {
        c = fgetc(fp);
        if(c == '\n')
        {
            webpage[head++] = '\r'; //putting \r in front of every \n
            webpage[head++] = '\n';
        }
        else
            webpage[head++] = c; //putting each char from .html file into webpage
            
        if( feof(fp) ) { 
            break ;
        }
    }
    fclose(fp);
   
   
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client, sock_user;

    char buf[2048];

    // next part used to get IP address of user
    char hostbuffer[256];
    char *IPbuffer;
    int hostname;
    struct hostent *host_entry;
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    host_entry = gethostbyname(hostbuffer);
    IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    // IPbuffer containes IP addres of user

    int on = 1; //for setsockopt - reuse of a socket
    
    //creating our socket
    if((fd_server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error("socket");
    
    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  inet_addr(IPbuffer); //servers IP address
    server_addr.sin_port = htons(PORT); //setting the port number to 8080

    //binding with the socket
    if(bind(fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    error("bind");

    printf("to connect with the client to this server\n");
    printf("use this IP address and port as the first argument\n");
    printf("%s:%d\n", IPbuffer, PORT);
    //listening
    if(listen(fd_server, 100) == -1)
        error("listen");


while(1)
{
    //accepting clients to connect to our server
    if((fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len)) == -1)
        error("accept");

    printf("Got client connection...\n");
    
    printf("\nConnected client's IP address: %s\n\n", inet_ntoa(client_addr.sin_addr)); //printing the IP address of the client 
    
    if(!fork())
    {
        close(fd_server);
        memset(buf, 0, 2048);
        read(fd_client, buf, 2047);
        printf("%s\n", buf);

        write(fd_client, webpage, sizeof(webpage) - 1);  
        
        close(fd_client);
        printf("closing...\n");
        exit(0);
    }

    close(fd_client);

}
return 0;
}
