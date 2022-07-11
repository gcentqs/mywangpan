#include "../include/thread_pool.h"

int tcp_init(int *sockfd, char *ipaddr, char *port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
    serv_addr.sin_port = htons(atoi(port));

    int ret = 0;
    int reuse = 1;
    ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    *sockfd = sfd;
    
    return 0;
}

