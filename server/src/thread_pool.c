#include "../include/thread_pool.h"

void *thread_func(void *p)
{
    pThread_pool_t pPool = (pThread_pool_t)p;
    pNode_t pCur;
    while (1)
    {
        pthread_mutex_lock(&pPool->que.mutex);
        while (0 == pPool->que.que_size)
        {
            printf("child wait\n");
            pthread_cond_wait(&pPool->que.cond, &pPool->que.mutex);
            printf("child wake up\n");
        }
        int ret = de_workqueue(&pPool->que, &pCur);
        pthread_mutex_unlock(&pPool->que.mutex);
        if (0 == ret)
        {
            task_process(pCur->client_fd, pPool);
            close(pCur->client_fd);
            free(pCur);
            pCur = NULL;
        }
    }

    pthread_exit(NULL);
}

int thread_pool_init(pThread_pool_t pPool, int thread_num)
{
    memset(pPool, 0, sizeof(Thread_pool_t));
    pPool->thread_num = thread_num;
    pPool->start_flag = 0;
    pPool->pthid = (pthread_t *)calloc(thread_num, sizeof(pthread_t));
    if (NULL == pPool->pthid)
    {
        printf("calloc error\n");
        exit(-1);
    }
    workqueue_init(&pPool->que);
    usr_queue_init(&pPool->usr_que);

    return 0;
}

int thread_pool_start(pThread_pool_t pPool)
{
    if (1 == pPool->start_flag)
    {
        return 0;
    }
    for (int i = 0; i < pPool->thread_num; ++i)
    {
        pthread_create(pPool->pthid+i, NULL, thread_func, pPool);
    }
}

