/*************************************************************************
    > File Name: echo_client.c
    > Author: PantaSun
    > Mail: sunsky2333@gmail.com
    > Created Time: Wed 29 May 2019 12:02:57 PM CST
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tlpi_hdr.h"
#include <string.h>

#define MAXSTRLEN 100
int main(int argc, char const *argv[])
{
    if( argc != 3)
        usageErr("usage: %s <IP> <port>\n", argv[0]);

    int str_len;
    int recv_len;
    int recv_cnt;
    char message[MAXSTRLEN];
    int sock;
    struct sockaddr_in serv_addr;

    sock =socket(PF_INET, SOCK_STREAM, 0);
    if( sock == -1)
        errExit("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) ==-1)
        errExit("connect");

    while (1)
    {
        fputs("Send Message(Q/q for quit): ", stdout);
        fgets(message, MAXSTRLEN, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        while(recv_len < str_len){
            recv_cnt = read(sock, &message[recv_len], MAXSTRLEN-1);
            if(recv_cnt == -1)
                errExit（"read");
            recv_len += recv_cnt;
        }
        message[recv_len] = '\0';
        printf("Receive Message from Server: %s \n", message);

    }
    close(sock);
    return 0;
}
