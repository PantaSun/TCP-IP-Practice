#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "tlpi_hdr.h"
#include <stdio.h>
#define MAXSTRLEN 100

int main(int argc, char const *argv[])
{
    if(argc != 2)
        usageErr("usage: %s <port> \n", argv[0]);


    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[MAXSTRLEN];
    int str_len;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        errExit("socket");
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        errExit("bind");
    
    if(listen(serv_sock, 5) == -1)
        errExit("listen");
    
    clnt_addr_size = sizeof(clnt_addr);
    for (int i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1){
            printf("accept client %d error.\n",i);
            continue;
        }
        else
            printf("Connected client %d.\n", i);
        
        
        while( (str_len = read(clnt_sock, message, MAXSTRLEN)) != 0)
            write(clnt_sock, message, str_len);
        
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
