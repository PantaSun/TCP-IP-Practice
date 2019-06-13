/*************************************************************************
	> File Name: thread1.c
	> Author: 
	> Mail: 
	> Created Time: Thu 30 May 2019 11:14:52 AM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include"tlpi_hdr.h"

void * thread_main(void * arg);

int main(int argc, char * argv[]){

    pthread_t pid;
    int thread_param = 5;
    void * thr_ret;

    if(pthread_create(&pid, NULL, thread_main, (void *)&thread_param) != 0)
        errExit("pthread_create");
    
    if(pthread_join(pid, &thr_ret) != 0)
        errExit("pthread_join");

    printf("Thread return message: %s\n",(char*)thr_ret);
    free(thr_ret);

    puts("end of man");
    return 0;
}

void * thread_main(void * arg){
    int cnt = *((int *)arg);
    char *msg = (char*)malloc(sizeof(char*)*50);
    strcpy(msg, "Hello I'm thread!\n");
    for(int i=0; i< cnt ; i++){
        sleep(1);
        puts("runing thread");
    }
    return (void*)msg;
}
