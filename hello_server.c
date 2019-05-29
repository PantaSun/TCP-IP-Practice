#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include "tlpi_hdr.h"
#include <arpa/inet.h>
int main(int argc, char const *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if(argc != 2)
        usageErr("usage: %s <port>\n", argv[0]);
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        errExit("socket");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        errExit("bind");
    if(listen(serv_sock, 5) == -1)
        errExit("listen");
    clnt_addr_size = sizeof(clnt_addr);

    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        errExit("accept");
    write(clnt_sock, message, sizeof(message));
    close(serv_sock);
    close(clnt_sock);
    

    return 0;
}
