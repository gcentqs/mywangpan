#ifndef __USR_INFO_H__
#define __USR_INFO_H__

#define USR_NAME_SIZE 20
#define USR_PWD_SIZE 20

typedef struct info_node {
    char usr_name[USR_NAME_SIZE];
    char usr_pwd[USR_PWD_SIZE];
    struct info_node *pNext;
} Usr_account_t, *pUsr_account_t;

typedef struct {
    pUsr_account_t pHead, pTail;
    int que_size;
} Queue_usr_t, *pQueue_usr_t;

int usr_queue_init(pQueue_usr_t);
int usr_account_insert(pQueue_usr_t, char *, char *);
int account_confirm(pQueue_usr_t, char *, char *);
int usr_log_in(pQueue_usr_t,  int, char *);

#endif
