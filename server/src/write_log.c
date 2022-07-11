#include "../include/thread_pool.h"

int write_log(char *filename, char *buf, int length)
{
    int fd = open(filename, O_RDWR|O_CREAT|O_APPEND, 0666);
    ERROR_CHECK(fd, -1, "open write_log");

    int ret = write(fd, buf, length);
    ERROR_CHECK(ret, -1, "write");
    
    close(fd);
    return 0;
}

