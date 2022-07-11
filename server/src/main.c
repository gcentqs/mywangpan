#include "../include/head.h"
#include "../include/work_que.h"
#include "../include/thread_pool.h"
#include "../include/usr_info.h"

int main(int argc, char *argv[])
{
    char (*args)[ARGS_SIZE];
    if ((args = (char(*)[ARGS_SIZE])malloc(sizeof(char)*ARGS_SIZE*3)) == NULL)
    {
        printf("malloc error\n");
        exit(-1);
    }
    memset(args, 0, sizeof(char)*ARGS_SIZE*3);
    determin_args(3, args);

    int thread_num = atoi(args[2]);
    pThread_pool_t pPool;
    if ((pPool = (pThread_pool_t)calloc(1, sizeof(Thread_pool_t))) == NULL)
    {
        printf("calloc error\n");
        exit(-1);
    }
    sys_init(pPool, thread_num);

    int sfd = 0;
    tcp_init(&sfd, args[0], args[1]);

    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    epoll_add(epfd, sfd);

    int rdynum = 0;
    int client_fd = 0;
    struct epoll_event evts[3];
    memset(evts, 0, sizeof(evts));

    while (1)
    {
        rdynum = epoll_wait(epfd, evts, 1, -1);
        
        for (int i = 0; i < rdynum; ++i)
        {
            if (evts[i].data.fd == sfd && evts[i].events == EPOLLIN)
            {
                client_fd = accept(sfd, NULL, NULL);
                ERROR_CHECK(client_fd, -1, "accept");

                pthread_mutex_lock(&pPool->que.mutex);
                en_workqueue(&pPool->que, client_fd);
                pthread_mutex_unlock(&pPool->que.mutex);
                pthread_cond_signal(&pPool->que.cond);
            }
        }
    }

    close(sfd);
    return 0;
}

