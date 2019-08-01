/*************************************************************************
  > File Name: echo_EPETserv.c
  > Author: 
  > Mail: 
  > 使用边缘触发模型的epoll IO多路复用
  > Created Time: Wed 29 May 2019 11:26:05 AM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include"tlpi_hdr.h"
#include<fcntl.h>
#include<errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

// 设置文件描述符为非阻塞的
void set_fd_nonblock(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

int main(int argc, char* argv[]){

    if(argc != 2)
        usageErr("usage: %s <port>\n",argv[0]);

    // socket argement 
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    int str_len;
    char buf[BUF_SIZE];

    // socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        errExit("socket");

    // set serv_addr
    memset(&serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // binding
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        errExit("bind");

    // listen
    if(listen(serv_sock, 5) == -1)
        errExit("listen");

    // epoll argement
    struct epoll_event event;
    struct epoll_event *ep_events;
    int efd, event_cnt;

    // set epoll

    efd = epoll_create(EPOLL_SIZE);
    ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    set_fd_nonblock(serv_sock);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(efd, EPOLL_CTL_ADD, serv_sock, &event);

    while(1){
        event_cnt = epoll_wait(efd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1)
            errExit("epoll_wait");
        puts("epoll_wait return");
        for(int i=0; i < event_cnt; i++){
            if(ep_events[i].data.fd == serv_sock)
            {
                clnt_addr_size = sizeof(clnt_sock);
                clnt_sock =accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                set_fd_nonblock(clnt_sock);
                event.events = EPOLLIN | EPOLLET;  // 边缘触发
                event.data.fd = clnt_sock;
                epoll_ctl(efd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d\n", clnt_sock);
            }
            else
            {
                
                while (1)
                {
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);

                    if(str_len == 0) // close client
                    {
                        epoll_ctl(efd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d\n",ep_events[i].data.fd);
                        break;
                    }
                    else if(str_len < 0) // 未读取到数据
                    {
                        if(errno == EAGAIN) // 数据读取完成
                            break;
                        else // 读取出错
                            errExit("read"); 

                    }
                    else
                    {
                        write(ep_events[i].data.fd, buf, str_len); // echo
                        buf[str_len] = '\0';
                        printf("receive from client: %d, message: %s\n", 
                                ep_events[i].data.fd, buf);
                    }
                }

            }
        }
    }
    close(serv_sock);
    close(efd);
    return 0;
}

