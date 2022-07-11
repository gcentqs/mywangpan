#include "../include/thread_pool.h"

int usr_queue_init(pQueue_usr_t pQueue_usr)
{
    memset(pQueue_usr, 0, sizeof(Queue_usr_t));

    return 0;
}

int usr_account_insert(pQueue_usr_t pQueue_usr, char *usr_name, char *usr_pwd)
{
    pUsr_account_t pNew;
    if ((pNew = (pUsr_account_t)calloc(1, sizeof(Usr_account_t))) == NULL)
    {
        printf("calloc error\n");
        exit(-1);
    }
    strcpy(pNew->usr_name, usr_name);
    strcpy(pNew->usr_pwd, usr_pwd);
    if (0 == pQueue_usr->que_size)
    {
        pQueue_usr->pHead = pNew;
        pQueue_usr->pTail = pNew;
    }
    else
    {
        pQueue_usr->pTail->pNext = pNew;
        pQueue_usr->pTail = pNew;
    }
    ++pQueue_usr->que_size;
    return 0;
}

int account_confirm(pQueue_usr_t pQueue_usr, char *usr_name, char *usr_pwd)
{
    pUsr_account_t pCur = pQueue_usr->pHead;
    while (pCur)
    {
        if (strcmp(pCur->usr_name, usr_name) == 0)
        {
            if (strcmp(pCur->usr_pwd, usr_pwd) == 0)
            {
                printf("welcome!\n");
                return 1;
            }
            else
            {
                printf("wrong password\n");
                return -1;
            }
            
        }
        pCur = pCur->pNext;
        
    }
    if (NULL == pCur)
        printf("account does not exist\n");
    return -1;
}

int usr_log_in(pQueue_usr_t pQueue_usr, int client_fd, char *clientname)
{
    char usr_name[USR_NAME_SIZE] = {0};
    char usr_pwd[USR_PWD_SIZE] = {0};

    train_t train;
    memset(&train, 0, sizeof(train));
    int data_len;
    int log_flag = 0;
    int ret = 0;

    while (1)
    {
        memset(usr_name, 0, USR_NAME_SIZE);
        memset(usr_pwd, 0, USR_PWD_SIZE);

        ret = recv(client_fd, &data_len, sizeof(int), 0);
        if (0 == ret)
        {
            break;
        }
        recv(client_fd, usr_name, data_len, 0);

        ret = recv(client_fd, &data_len, sizeof(int), 0);
        if (0 == ret)
        {
            break;
        }
        recv(client_fd, usr_pwd, data_len, 0);

        ret = account_confirm(pQueue_usr, usr_name, usr_pwd);

        train.length = sizeof(int);
        if (1 == ret)
        {
            log_flag = 1;
        }
        memcpy(train.buf, &log_flag, train.length);
        send(client_fd, &train, sizeof(int)+train.length, 0);

        if (1 == ret)
        {
            strcpy(clientname, usr_name);
            break;
        }
    }
}
