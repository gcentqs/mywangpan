#include "../include/thread_pool.h"

char *determin_mode(mode_t mode, char *str) //根据mode确定文件类型以及权限
{
    int k = 0;
    mode_t mode_low = mode & 0777;//mode最低的9位是权限
    mode_t mode_high = mode >> 9;
    if (mode_high == 040)
    {
        str[k++] = 'd';
    }
    else if (mode_high == 0100)
    {
        str[k++] = '-';//普通文件
    }
    int pos = 0x100;
    for (int i = 0; i < 9; ++i)
    {
        if (pos & mode_low)
        {
            if (i % 3 == 0)
            {
                str[k++] = 'r';
            }
            else if (i % 3 == 1)
            {
                str[k++] = 'w';
            }
            else
            {
                str[k++] = 'x';
            }
        }
        else
        {
            str[k++] = '-';
        }
        pos >>= 1;
    }
    return str;
}
int myls(char *pathname)
{
    char pwdtmp[256] = {0};

    DIR *dirp = opendir(pathname);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *pdirent;
    while (pdirent = readdir(dirp))
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }
        struct stat buf;
        char str[20] = {0};
        memset(pwdtmp, 0, sizeof(pwdtmp));
        strcpy(pwdtmp, pathname);
        pwdtmp[strlen(pwdtmp)] = '/';
        strcat(pwdtmp, pdirent->d_name);
        int ret = stat(pwdtmp , &buf);
        ERROR_CHECK(ret, -1, "stat");
        printf("%10s %5ld %s %s %9ld %20s %s"
               , determin_mode(buf.st_mode, str)
               , buf.st_nlink
               , getpwuid(buf.st_uid)->pw_name
               , getgrgid(buf.st_gid)->gr_name
               , buf.st_size
               , pdirent->d_name
               , ctime(&buf.st_mtime)
              );
    }
    closedir(dirp);
    return 0;
}

