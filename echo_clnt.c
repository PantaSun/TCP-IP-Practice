/*************************************************************************
	> File Name: echo_clnt.c
	> Author: 
	> Mail: 
	> Created Time: Wed 29 May 2019 12:02:57 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"tlpi_hdr.h"
#define BUF_SIZE 100

int main(int argc, char*argv[]){
    if(argc != 3)
        usageErr("usage: %s <IP> <port>\n", argv[0]);

    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        errExit("sock");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        errExit("connect");
    else
        puts("Connected....");
    while(1){
        fputs("Input message(Q/q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(!strcmp(message, "q\n")|| !strcmp(message, "Q\n"))
            break;

        str_len = write(sock, message, strlen(message));

        recv_len = 0;
        while(recv_len < str_len){
            recv_cnt =read(sock, message, BUF_SIZE);
            if(recv_cnt == -1)
                errExit("read");
            recv_len += recv_cnt;
        }
        message[recv_len] = '\0';
        printf("Message from serv: %s\n", message);

    }
    close(sock);
    return 0;

}
