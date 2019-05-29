#include <sys/socket.h>
#include <arpa/inet.h>
#include "tlpi_hdr.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[50];
    int str_len;
    if(argc != 3)
        usageErr("uasge: %s <IP> <port>\n", argv[0]);
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        errExit("socket");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        errExit("connect");
    str_len = read(sock, message, sizeof(message)-1);
    if(str_len == -1)
        errExit("read");
    printf("Message from server: %s\n", message);
    close(sock);
    return 0;
}
