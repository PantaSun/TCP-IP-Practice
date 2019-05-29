#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "tlpi_hdr.h"
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#define MAXSTRLEN 100

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

 
    // select参数
    fd_set reads;
    fd_set cpy_reads;
    int fd_max;
    int fd_num;
    struct timeval timeout;
    
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

    // 设置fd_set
    FD_ZERO(&reads);
    // 将serv_sock加入到reads中
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    
    puts("Waiting for accept...");
    while (1)
    {
        // 拷贝reads
        cpy_reads = reads;
        // 设置超时
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout);
        if (fd_num == -1)
            break;
        else if( fd_num == 0)
            continue;

        // 判断哪个fd发生事件
        for (int i = 0; i < fd_max+1; i++)
        {
            if(FD_ISSET(i, &cpy_reads)){
                // 如果serv_sock发生事件，说明有新的建立连接请求
                if (i == serv_sock)
                {
                    clnt_addr_size = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                    if(clnt_sock == -1){
                        puts("accept error");
                        printf("%d\n", clnt_sock);
                        continue;
                    }
                    else
                    {
                        printf("Client %d has been added\n", clnt_sock);
                    }
                    
                    // 将新建立的fd加入的reads中进行监视
                    FD_SET(clnt_sock, &reads);
                    // 修改fd_max
                    if(fd_max < clnt_sock)
                        fd_max = clnt_sock;
                }
                else // 如果是其他fd则进行读写操作
                {
                    str_len = read(i, message, MAXSTRLEN);
                    if(str_len == 0) // 读取0，说明连接已经关闭
                    {
                        // 从reads中移除该fd
                        FD_CLR(i, &reads);
                        close(i);
                        printf("close client: %d\n", i);
                    }
                    else if(str_len < 0)
                    {
                        printf("client: %d read error\n", i);
                        continue;
                    }else // echo
                    {
                        write(i, message, str_len);
                        message[str_len] = 0;
                        printf("Receive from client %d: %s", i, message);
                    }
                    
                }
                
                
            }
        }
                
        
    }

    close(serv_sock);
    return 0;
}
