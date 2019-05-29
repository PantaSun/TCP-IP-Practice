#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string.h>
#include "tlpi_hdr.h"
#include <stdio.h>
#include <signal.h>
#define MAXSTRLEN 100
/*
 * 子进程退出信号处理程序 
 */
void deal_childproc(int sig){
    pid_t pid;
    pid = waitpid(-1, NULL, WNOHANG);
    if(pid == -1)
        errExit("waitpid");
    else
    {
        printf("remove child proc: %d\n", pid);
    }
    
}
int main(int argc, char const *argv[])
{
    if(argc != 2)
        usageErr("usage: %s <port> \n", argv[0]);
    // 进程相关参数
    pid_t pid;

    // socket相关参数
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[MAXSTRLEN];
    int str_len;
    int clnt_count;

    // 注册子进程退出信号处理程序，防止僵尸进程
    struct sigaction act;
    act.sa_handler = deal_childproc;
    act.sa_flags = 0;
    if( sigemptyset(&act.sa_mask) == -1)
        errExit("sigemptyset");
    
    if( sigaction(SIGCHLD, &act, 0) == -1)
        errExit("sigaction");
    
    // 创建服务端套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        errExit("socket");
    
    // 绑定IP地址和端口号
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        errExit("bind");
    
    // 监听状态
    if(listen(serv_sock, 5) == -1)
        errExit("listen");
    clnt_addr_size = sizeof(clnt_addr);
    clnt_count = 0;
    while (1)
    {
        clnt_count++;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1){
            //printf("accept client %d error.\n",clnt_count);
            continue;
        }
        else
            printf("Connected client %d.\n", clnt_count);
        pid = fork();
        if(pid == -1){
            close(clnt_sock);
            continue;
        }
        else if(pid == 0)  // 子进程
        {
            // 先关闭父进程中服务器套接字，方便父进程关闭套接字
            close(serv_sock);
            while( (str_len = read(clnt_sock, message, MAXSTRLEN)) != 0){
                write(clnt_sock, message, str_len);
                message[str_len] = '\0';
                printf("receive from %d client: %s\n", clnt_count, message);

            }
        
            close(clnt_sock);
            puts("client disconnected.... ");
            return 0;
        }
        else
        {
            // 关闭父进程中建立连接套接字，方便子进程关闭套接字
            close(clnt_sock);
        }
        
        
    }

    close(serv_sock);
    return 0;
}
