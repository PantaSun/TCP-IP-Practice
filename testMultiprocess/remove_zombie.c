#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "apue.h"

void read_childproc(int sig){
    if (sig == SIGCHLD)
    {
        printf("a child process closed.\n");
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if(WIFEXITED(status))
        {
            printf("Removed proc id: %d\n", pid);
            printf("Child send: %d\n", WEXITSTATUS(status));
        }

    }
    
}


int main(int argc, char const *argv[])
{
    /* code */
    pid_t pid;

    struct sigaction act;
    act.sa_handler = read_childproc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    if(sigaction(SIGCHLD, &act, 0) == -1)
        err_sys("sigaction");

    pid = fork();

    if(pid < 0)
        err_sys("fork");
    else if(pid == 0){
        puts("Hi! I'm child process");
        sleep(10);
        return 12;
    }
    else
    {
        printf("child proc id: %d\n", pid);
        pid = fork();
        if (pid < 0)
            err_sys("fork2");
        else if(pid == 0)
        {
            puts("Hi! I'm child process");
            sleep(10);
            return 14;
        }
        else
        {
            printf("child pro id: %d\n", pid);
            for (int i = 0; i < 5; i++)
            {
                puts("wait...");
                sleep(5);
            }
            
        }
        
    }
    
    return 0;
}
