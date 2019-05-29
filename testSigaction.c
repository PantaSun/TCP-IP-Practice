#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void timeout(int sig){

    if( sig == SIGALRM)
        printf("time out.\n");
    alarm(2);
}
int main(int argc, char const *argv[])
{
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    alarm(2);
    for (int i = 0; i < 3; i++)
    {
        puts("sleeping 100");
        sleep(100);
    }

    return 0;
}
