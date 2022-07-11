#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "head.h"
#include "work_que.h"
#include "usr_info.h"
#define ARGS_SIZE 20

typedef struct {
    int thread_num;
    pthread_t *pthid;
    Queue_t que;
    int start_flag;
    Queue_usr_t usr_que;
} Thread_pool_t, *pThread_pool_t;

typedef struct {
    int length;
    char buf[1024];
} train_t;

int determin_args(int, char (*)[ARGS_SIZE]);
int determin_path(char *, char *);
int thread_pool_init(pThread_pool_t, int);
int thread_pool_start(pThread_pool_t);
int sys_init(pThread_pool_t, int);
int tcp_init(int *, char *, char *);
int epoll_add(int, int);
int parseline(char *, char **);
int task_process(int, pThread_pool_t);
int myls(char *);
int remove_dir(char *);
int mytree(char *);
int puts_file(int, char *);
int gets_file(int, char *);
int write_log(char *, char *, int);

#endif
