#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "tlpi_hdr.h"
#include <stdio.h>
#define MAXSTRLEN 100
#define INTSIZE sizeof(int)

int calculate(int count, int *message, char op){

    if(count < 1 || count > 10)
        return 0;
    int result = message[0];
    printf("%d ", result);

    switch (op)
    {
    case '+':
        for (int i = 1; i < count; i++){
            printf(" %c ", op);
            printf(" %d ", message[i]);
            
            result += message[i];
        }
        printf(" %c ", '=');
        break;
    case '-':
        for (int i = 1; i < count; i++){
            printf(" %c ", op);
            printf(" %d ", message[i]);
            
            result -= message[i];
        }
        printf(" %c ", '='); 
        break;
    case '*':
        for (int i = 1; i < count; i++){
            printf(" %c ", op);
            printf(" %d ", message[i]);
            
            result *= message[i];
        }
        printf(" %c ", '=');
        break;
    case '/':
        for (int i = 1; i < count; i++){
            printf(" %c ", op);
            printf(" %d ", message[i]);
            
            result /= message[i];
        }
        printf(" %c ", '=');           
    default:
        break;
    }
    printf(" %d\n ", result);

    return result;
}

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
    int number_count;
    int recv_cnt;
    int recv_len;
    int str_len;

    int result;
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
    else
    {
        printf("listening for accept....\n");
    }
    
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
        
        while (1)
        {
            while((str_len = read(clnt_sock, &number_count, 1)) == 0) 
                if(str_len == -1)
                    break;
            
            recv_len = 0;
            while (recv_len <(number_count *INTSIZE + 1))
            {
                recv_cnt = read(clnt_sock, &message[recv_len], MAXSTRLEN-1);
                recv_len += recv_cnt;
            }

            result = calculate(number_count, (int *)&message, message[recv_len-1]);

            
            write(clnt_sock, &result, INTSIZE);
            result = 0;
        }
        
        
        
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
