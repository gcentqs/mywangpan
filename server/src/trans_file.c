#include "../include/thread_pool.h"
#define THREAD_SIZE (1<<20)*100

int recv_circle(int fd, void *buf, int data_len);
int recv_circle_file(int fd, void *buf, int data_len, off_t localsize, int fd_info);

int puts_file(int client_fd, char *filename)
{
    int fd = open(filename, O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    train_t train;
    memset(&train, 0, sizeof(train));

    off_t localsize = 0;
    int data_len;
    int ret = 0;

    //发送文件大小
    struct stat file_stat;
    memset(&file_stat, 0, sizeof(file_stat));
    ret = fstat(fd, &file_stat);
    ERROR_CHECK(ret, -1, "fstat");

    train.length = sizeof(file_stat.st_size);
    memcpy(train.buf, &file_stat.st_size, train.length);
    printf("filesize = %ld\n", file_stat.st_size);
    ret = send(client_fd, &train, sizeof(int)+train.length, 0);
    ERROR_CHECK(ret, -1, "send filesize");

    //接收对方本地的文件大小，用于断点续传
    ret = recv(client_fd, &data_len, sizeof(int), 0);
    ERROR_CHECK(ret, -1, "recv");
    ret = recv(client_fd, &localsize, data_len, 0);
    ERROR_CHECK(ret, -1, "recv");
    printf("local size from duifang is %ld\n", localsize);

    if (file_stat.st_size <= THREAD_SIZE)
    {
        lseek(fd, localsize, SEEK_CUR);
        while (1)
        {
            ret = read(fd, train.buf, sizeof(train.buf));
            ERROR_CHECK(ret, -1, "read");
            train.length = ret;

            ret = send(client_fd, &train, sizeof(int)+train.length, 0);
            if (-1 == ret)
            {
                printf("client disconnected\n");
                break;
            }
            if (0 == train.length)
            {
                break;
            }
        }
    }
    else
    {
        printf("trans big data\n");
        char *pMap = (char *)mmap(NULL, file_stat.st_size, PROT_READ|PROT_WRITE,
                                  MAP_SHARED, fd, 0);
        ERROR_CHECK(pMap, MAP_FAILED, "mmap");
        ret = send(client_fd, pMap+localsize, file_stat.st_size-localsize, 0);
        if (-1 == ret)
        {
            printf("client disconnected\n");
        }
        munmap(pMap, file_stat.st_size);
    }

    close(fd);
    return 0;
}

int gets_file(int client_fd, char *filename)
{
    int fd = open(filename, O_RDWR|O_CREAT|O_APPEND, 0666);
    ERROR_CHECK(fd, -1, "open");

    int ret = 0;

    int data_len = 0;
    char buf[1024];
    //接收文件大小
    off_t filesize = 0;
    ret = recv_circle(client_fd, &data_len, sizeof(int));
    ERROR_CHECK(ret, -1, "recv filesize");    
    ret = recv_circle(client_fd, &filesize, data_len);
    ERROR_CHECK(ret, -1, "recv filesize");

    printf("filesize = %ld\n", filesize);

    if (filesize <= THREAD_SIZE)
    {
        //发送本地文件大小
        struct stat file_stat;
        memset(&file_stat, 0, sizeof(file_stat));
        ret = fstat(fd, &file_stat);
        ERROR_CHECK(ret, -1, "fstat");
        train_t train;
        memset(&train, 0, sizeof(train));
        train.length = sizeof(file_stat.st_size);
        memcpy(train.buf, &file_stat.st_size, train.length);
        printf("locao filesize = %ld\n", file_stat.st_size);
        ret = send(client_fd, &train, sizeof(int)+train.length, 0);
        ERROR_CHECK(ret, -1, "send");

        while (1)
        {
            ret = recv_circle(client_fd, &data_len, sizeof(int));
            ERROR_CHECK(ret, -1, "recv_circle");
            if (0 == data_len)
            {
                break;
            }
            ret = recv_circle(client_fd, buf, data_len);
            if (-1 == ret)
            {
                printf("client disconnected\n");
                return -1;
            }
            ret = write(fd, buf, data_len);
            ERROR_CHECK(ret, -1, "write");
        }

    }
    else
    {
        off_t localsize = 0;
        int fd_info = 0;
        char infofile[256] = {0};
        sprintf(infofile, "%s%s", filename, "info");
        if ((ret = open(infofile, O_RDWR)) == -1)
        {
            fd_info = open(infofile, O_RDWR|O_CREAT, 0666);
            ERROR_CHECK(fd_info, -1, "open infofile");
            write(fd_info, &localsize, sizeof(off_t));//将当前文件大小写回infofile
        }
        else
        {
            fd_info = ret;
            read(fd_info, &localsize, sizeof(off_t));
        }
        train_t train;
        memset(&train, 0, sizeof(train));
        train.length = sizeof(off_t);
        memcpy(train.buf, &localsize, train.length);
        printf("local filesize = %ld\n", localsize);
        ret = send(client_fd, &train, sizeof(int)+train.length, 0);
        ERROR_CHECK(ret, -1, "send");

        ret = ftruncate(fd, filesize);
        ERROR_CHECK(ret, -1, "ftruncate");
        char *pMap = (char *)mmap(NULL, filesize, PROT_READ|PROT_WRITE,
                                  MAP_SHARED, fd, 0);
        ERROR_CHECK(pMap, MAP_FAILED, "mmap");
        ret = recv_circle_file(client_fd, pMap, filesize, localsize, fd_info);
        if (ret < filesize)
        {
            printf("disconnect\n");
        }
        else if (ret == filesize)
        {
            remove(infofile);   //文件传输完成，将辅助文件删除
        }
        munmap(pMap, filesize);
        close(fd_info);
    }
    close(fd);
}

int recv_circle(int fd, void *buf, int data_len)
{
    int len = 0;
    int total = 0;
    char *p = (char *)buf;
    while (total < data_len)
    {
        len = recv(fd, p+total, data_len-total, 0);
        total += len;
        if (0 == len)
        {
            break;
        }
    }

    return total;
}
int recv_circle_file(int fd, void *buf, int data_len, off_t localsize, int fd_info)
{
    int len = 0;
    char *p = (char *)buf;
    while (localsize < data_len)
    {
        len = recv(fd, p+localsize, data_len-localsize, 0);
        localsize += len;
        lseek(fd_info, 0, SEEK_SET);
        write(fd_info, &localsize, sizeof(off_t));
        if (0 == len)
        {
            break;
        }
    }

    return localsize;
}
