#include "../include/thread_pool.h"

int sys_init(pThread_pool_t pPool, int thread_num)
{
    thread_pool_init(pPool, thread_num);
    thread_pool_start(pPool);

    FILE *fp = fopen("../conf/usr_account", "r");
    ERROR_CHECK(fp, NULL, "fopen");

    char usr_name[USR_NAME_SIZE] = {0};
    char usr_pwd[USR_PWD_SIZE] = {0};

    while (fscanf(fp, "%s%s", usr_name, usr_pwd) != EOF)
    {
        usr_account_insert(&pPool->usr_que, usr_name, usr_pwd);
        memset(usr_name, 0, sizeof(usr_name));
        memset(usr_pwd, 0, sizeof(usr_pwd));
    }

    return 0;
}

