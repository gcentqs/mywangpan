#include "../include/work_que.h"

int workqueue_init(pQueue_t pQueue)
{
    memset(pQueue, 0, sizeof(Queue_t));
    pthread_mutex_init(&pQueue->mutex, NULL);
    pthread_cond_init(&pQueue->cond, NULL); 

    return 0;
}

int en_workqueue(pQueue_t pQueue, int client_fd)
{
    pNode_t pNew;
    if ((pNew = (pNode_t)calloc(1, sizeof(Node_t))) == NULL)
    {
        printf("calloc error\n");
        return -1;
    }
    pNew->client_fd = client_fd;
    if (0 == pQueue->que_size)
    {
        pQueue->pHead = pNew;
        pQueue->pTail = pNew;
    }
    else
    {
        pQueue->pTail->pNext = pNew;
        pQueue->pTail = pNew;
    }
    ++pQueue->que_size;
    return 0;
}

int de_workqueue(pQueue_t pQueue, pNode_t *ppNode)
{
    if (0 == pQueue->que_size)
    {
        return -1;
    }
    else if (1 == pQueue->que_size) 
    {
        *ppNode = pQueue->pHead;
        pQueue->pHead = NULL;
        pQueue->pTail = NULL;
    }
    else
    {
        *ppNode = pQueue->pHead;
        pQueue->pHead = pQueue->pHead->pNext;
    }
    --pQueue->que_size;
    return 0;
}
