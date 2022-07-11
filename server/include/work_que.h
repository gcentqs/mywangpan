#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__

#include "head.h"
typedef struct node {
    int client_fd;
    struct node *pNext;
} Node_t, *pNode_t;

typedef struct {
    pNode_t pHead, pTail;
    int que_size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue_t, *pQueue_t;

int workqueue_init(pQueue_t);
int en_workqueue(pQueue_t, int);
int de_workqueue(pQueue_t, pNode_t *);

#endif
