#include <stdio.h>
#include <netdb.h>
#include "tlpi_hdr.h"
#include <arpa/inet.h>
int main(int argc, char const *argv[])
{
    if(argc != 2)
        usageErr("usage: %s <addr>\n", argv[0]);
    struct hostent *host;

    host = gethostbyname(argv[1]);
    if(host == NULL)
        errExit("gethostbyname");
    
    printf("Office name: %s\n", host->h_name);
    for (int i = 0; host->h_aliases[i]; i++)
    {
        printf("Aliases %d : %s\n", i+1, host->h_aliases[i]);
    }

    printf("Address types: %s\n", host->h_addrtype==AF_INET?"AF_INET":"AF_INET6");

    for (int i = 0; host->h_addr_list[i]; i++)
    {
        printf("IP addr %d: %s\n", i+1, 
                inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    
    
    return 0;
}
