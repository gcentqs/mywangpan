#include "../include/myfunc.h"
#define CMD_NUM 10

void find_filename(char *arg, char **filename)
{
    int i = strlen(arg)-1;        
    for (; i >= 0 && arg[i] != '/'; --i)
    {
        ;
    }
    *filename = arg+i+1;
}

int main()
{
    char (*args)[ARGS_SIZE];
    if ((args = (char(*)[ARGS_SIZE])malloc(sizeof(char)*2*ARGS_SIZE)) == NULL)
    {
        printf("malloc error\n");
        exit(-1);
    }
    determin_args(2, args);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(server_fd, -1, "socket");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(args[0]);
    serv_addr.sin_port = htons(atoi(args[1]));

    int ret = 0;
    ret = connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    ERROR_CHECK(ret, -1, "connect");

    usr_log_in(server_fd);

    int argc = 0;
    char cmdline[128] = {0};
    char *argv[CMD_NUM] = {0};

    char pathname[256] = {0};

    train_t train;
    memset(&train, 0, sizeof(train));


    while (1)
    {
        memset(pathname, 0, sizeof(pathname));
        strcpy(pathname, getcwd(NULL, 0));

        strcpy(pathname, getcwd(NULL, 0));

        ret = read(STDIN_FILENO, train.buf, sizeof(train.buf));
        if (ret == 0)
        {
            break;
        }
        train.length = ret;
        if (train.buf[train.length-1] = '\n')
        {
            train.buf[train.length-1] = '\0';
            train.length--;
        }
        send(server_fd, &train, sizeof(int)+train.length, 0);

        argc = 0;
        memset(cmdline, 0, sizeof(cmdline));
        memset(argv, 0, sizeof(argv));
        strncpy(cmdline, train.buf, train.length);
        argc = parseline(cmdline, argv);
        if (strcmp(argv[0], "gets") == 0)
        {
            if (2 == argc)//下载到当前工作目录
            {
                char *filename = NULL;
                find_filename(argv[argc-1], &filename);
                sprintf(pathname, "%s%s%s", pathname, "/", filename);
                printf("pathname = %s\n", pathname);
                gets_file(server_fd, pathname);
                printf("trans end\n");
            }
        }
        else if (strcmp(argv[0], "puts") == 0)
        {
            if (2 == argc)
            {
                determin_path(pathname, argv[argc-1]);
                puts_file(server_fd, pathname);
            }
        }
        else if (strcmp(argv[0], "exit") == 0)
        {
            printf("\nclient exits\n");
            break;
            // exit(0);
        }
    }

    close(server_fd);
    return 0;
}

