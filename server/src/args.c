#include "../include/thread_pool.h"

int determin_args(int argnum, char (*args)[ARGS_SIZE])
{
    int fd = open("../conf/config", O_RDONLY);
    ERROR_CHECK(fd, -1, "open");

    char buf[1024] = {0};
    int ret = 0;
    while (ret = read(fd, buf, sizeof(buf)) > 0)
    {
        ;
    }
    if (-1 == ret)
    {
        printf("read error\n");
        exit(-1);
    }

    int j = 0;
    for (int i = 0; i < argnum; ++i)
    {
        int k = 0;
    
        while (buf[j] != ':')
        {
            ++j;
        }
        ++j;
        while (buf[j] == ' ')
        {
            ++j;
        }
        while (buf[j] != '\n')
        {
            args[i][k] = buf[j];
            ++k;
            ++j;
        }
        ++j;
    }

    close(fd);
    return 0;
}

