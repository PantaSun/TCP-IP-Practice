# 各种版本的echo服务端/客户端

程序中使用的errExit等函数只是一个出错处理函数，会打印出给定字符从参出并推出，该函数是在"tlpi_hdr.h"头文件中声明，这个函数也可以自己实现。如：直接用fprintf/printf等函数打印出错信息+return退出即可。

## 客户端

客户端是通用的有两个版本：

- [基本客户端](https://github.com/PantaSun/TCP-IP-Practice/blob/master/EchoServer/client/echo_client.c) 	（需要手动输入IP和端口）
-  [基本客户端2](https://github.com/PantaSun/TCP-IP-Practice/blob/master/EchoServer/client/echo_testclient.c)   （已经绑定IP: 127.0.0.1 端口: 8848）
## 服务端基本实现版

- [服务端](https://github.com/PantaSun/TCP-IP-Practice/blob/master/EchoServer/testSimple/echo_server.c)

## 多进程版本

## 多线程版本

## select版本

## poll版本

## epoll版本

- [epoll默认版](https://github.com/PantaSun/TCP-IP-Practice/blob/master/EchoServer/testEpoll/echo_epollserv.c) 

  默认情况下采用LT模式

- [epollLT模式版](https://github.com/PantaSun/TCP-IP-Practice/blob/master/EchoServer/testEpoll/echo_EPLTserv.c) 

  将用户内空间的buffer设置小一些（如4字节），这样read函数就可能无法将内核空间的buffer中的数据一次性读完。从而可以看出未读完的输入缓冲会继续多次触发事件。

-  [epollET模式版](https://github.com/PantaSun/TCP-IP-Practice/blob/master/EchoServer/testEpoll/echo_EPETserv.c)

   一定要采用非阻塞工作方式的套接字，因为在ET模式下需要循环将输入缓存清空（读取出来），若为阻塞模式会在输入缓存清空是阻塞进程，可能引起服务器长时间停顿，所以要采用非阻塞工作模式。
   
   采用非阻塞工作模式时，在输入缓冲为空时read函数会返回-1，而套接字相关函数在出错是也会返回-1，那么如何区分是读空了输入缓冲区还是出错了呢？这里就要用到errno全局变量。
   
   errno变量是在"error.h"头文件中extern声明的。每种函数出错时，在errno中保存的值都不同。
   
   read函数发现输入缓冲中没有数据可读时返回-1，同时在errno中保存EAGAIN变量。
   
   所以在判断read函数返回-1后，在判断errno是否等于EAGAIN就可以得知输入缓冲区是否为空，从而终止循环。