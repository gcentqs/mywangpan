#include "../include/thread_pool.h"
#define CMD_NUM 10
#define DIR_DEPTH 10

void find_filename(char *arg, char **filename)
{
    int i = strlen(arg)-1;        
    for (; i >= 0 && arg[i] != '/'; --i)
    {
        ;
    }
    *filename = arg+i+1;
}

int task_process(int client_fd, pThread_pool_t pPool)
{
    system("clear");
    //验证登录账户的信息
    char clientname[128] = {0};
    if (-1 == usr_log_in(&pPool->usr_que, client_fd, clientname))
    {
        return -1;
    }

    //创建日志
    char logname[256] = {0};
    char timebuf[128] = {0};
    char logbuf[256] = {0};
    time_t curtime = time(NULL);
    struct tm now;
    localtime_r(&curtime, &now);
    sprintf(timebuf, "%d%d%d-%d%d%d", now.tm_year+1900, now.tm_mon+1, now.tm_mday,
                                    now.tm_hour, now.tm_min, now.tm_sec);
    sprintf(logname, "%s%s%s%s", "../log/" , clientname, "_", timebuf);
    memset(timebuf, 0, sizeof(timebuf));
    strcpy(timebuf, ctime(&curtime));
    sprintf(logbuf, "%s%s\n", "log in time: ", timebuf);
    /* printf("logname = %s\n", logname); */
    /* printf("timebuf = %s\n", timebuf); */
    write_log(logname, logbuf, strlen(logbuf));

    train_t train;
    memset(&train, 0, sizeof(train));

    char cmdline[128] = {0};
    char *argv[CMD_NUM] = {0};

    char pwd[256] = "/home/simon";
    char pathname[256] = {0};
    //用于保存上一级目录
    char (*prepwd)[256] = (char (*)[256])malloc(sizeof(char)*256*DIR_DEPTH);
    memset(prepwd, 0, sizeof(char)*256*DIR_DEPTH);
    int stack_top = 0;
    strcpy(prepwd[stack_top++], "/home/simon");

    int argc = 0;
    int ret = 0;
    int data_len = 0;

    while (1)
    {
        memset(pathname, 0, sizeof(pathname));
        strcpy(pathname, pwd);

        argc = 0;
        memset(cmdline, 0, sizeof(cmdline));
        memset(argv, 0, sizeof(argv));

        ret = recv(client_fd, &data_len, sizeof(int), 0);
        ERROR_CHECK(ret, -1, "recv");
        if (0 == ret)
        {
            break;
        }
        ret = recv(client_fd, train.buf, data_len, 0);
        ERROR_CHECK(ret, -1, "recv");
        strncpy(cmdline, train.buf, data_len);

        memset(logbuf, 0, sizeof(logbuf));
        memset(timebuf, 0, sizeof(timebuf));
        ctime_r(&curtime, timebuf);
        sprintf(logbuf, "%-30s\t%20s", cmdline, timebuf);
        write_log(logname, logbuf, strlen(logbuf));

        argc = parseline(cmdline, argv);//解析命令行
        if (0 == argc)
        {
            continue;
        }
        //根据命令，实现不同的功能
        if (0 == strcmp(argv[0], "ls"))
        {
            if (2 == argc)//ls  或者是ls pathname 都可以
            {
                puts(argv[argc-1]);
                determin_path(pathname, argv[argc-1]);
                myls(pathname);
            }
            else if (1 == argc)
            {
                myls(pathname);
            }
        }
        else if (0 == strcmp(argv[0], "remove"))
        {
            if (2 == argc)
            {
                determin_path(pathname, argv[argc-1]);
                remove_dir(pathname);
            }
        }
        else if (0 == strcmp(argv[0], "cd"))
        {
            if (2 == argc)
            {
                if (strcmp(argv[argc-1], "..") == 0)//返回到上级目录，需要用到栈
                {
                    memset(pathname, 0, sizeof(pathname));
                    strcpy(pathname, prepwd[stack_top]);
                    if (stack_top > 0)
                        --stack_top;
                }
                else
                {
                    determin_path(pathname, argv[argc-1]);
                    memset(prepwd[++stack_top], 0, 256);
                    strcpy(prepwd[stack_top], pwd);
                }
                DIR *dirp;
                if ((dirp = opendir(pathname)) == NULL)//目录不存在
                {
                    if (stack_top > 0)
                        --stack_top;
                    continue;
                }
                memset(pwd, 0, sizeof(pwd));
                strcpy(pwd, pathname);
                puts(pwd);
            }
        }
        else if (0 == strcmp(argv[0], "pwd"))
        {
            if (1 == argc)
            {
                determin_path(pathname, argv[argc-1]);
                puts(pwd);
            }
        }
        else if (0 == strcmp(argv[0], "mkdir"))
        {
            if (2 == argc)
            {
                determin_path(pathname, argv[argc-1]);
                mkdir(pathname, 0777);
            }
        }
        else if (0 == strcmp(argv[0], "tree"))
        {
            if (2 == argc)
            {
                determin_path(pathname, argv[argc-1]);
                mytree(pathname);
            }
            if (1 == argc)
            {
                mytree(pwd);
            }
        }
        else if (0 == strcmp(argv[0], "gets"))
        {
            if (2 == argc)
            {
                determin_path(pathname, argv[argc-1]);
                puts_file(client_fd, pathname);
                printf("trans end\n");
            }
        }
        else if (0 == strcmp(argv[0], "puts"))//上传到当前工作目录
        {
            if (2 == argc)
            {
                char *filename = NULL;
                find_filename(argv[argc-1], &filename);
                memset(pathname, 0, sizeof(pathname));
                sprintf(pathname, "%s%s%s", pwd, "/", filename);
                printf("pathname = %s\n", pathname);
                gets_file(client_fd, pathname);
                printf("trans end\n");
            }
        }
        else if (0 == strcmp(argv[0], "exit"))
        {
            if (1 == argc)
            {
                return 0;
            }
        }
        printf("\n");

    }

    return 0;
}

