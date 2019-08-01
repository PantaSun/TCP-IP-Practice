#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tlpi_hdr.h"
#include <string.h>

#define MAXSTRLEN 100
#define INTSIZE sizeof(int)
int main(int argc, char const *argv[])
{
    if( argc != 3)
        usageErr("usage: %s <IP> <port>\n", argv[0]);
    
    int number_count;
    int result;
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
    else
    {
        printf("Connected success! \n");
    }
    

    while (1)
    {

        fputs("Input number count:( >1 and < 10. other for quit): ", stdout);
        scanf("%d", &number_count);
        if(number_count > 10 || number_count < 1)
            break;
        message[0] = (char)number_count;
    
        for (int i = 0; i < number_count; i++)
        {
            printf("Operand %d: ", i+1);
            scanf("%d", (int *)&3message[INTSIZE*i+1]);
        }
        fgetc(stdin);
        fputs("Operator: ", stdout);
        scanf("%c", &message[number_count*INTSIZE+1]);
        
        if(write(sock, message, INTSIZE*number_count+2) < 0)
            errExit("write");

        if(read(sock, &result, INTSIZE) < 0)
            errExit("read");

        
        printf("Result is: %d \n", result);

    }
    close(sock);
    return 0;
}
