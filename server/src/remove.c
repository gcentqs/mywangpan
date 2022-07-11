#include "../include/thread_pool.h"

int remove_dir(char *pathname)
{
    struct stat statbuf;
    int ret = stat(pathname, &statbuf);
    ERROR_CHECK(ret, -1, "stat");

    if (statbuf.st_mode>>12 == 4)
    {
        DIR *dirp = opendir(pathname);
        ERROR_CHECK(dirp, NULL, "open");
        struct dirent *pdirent;
        while (pdirent = readdir(dirp))
        {
            if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..")==0)
            {
                continue;
            }
            else
            {
                char newpath[1024] = {0};
                sprintf(newpath, "%s%s%s", pathname, "/", pdirent->d_name);
                remove_dir(newpath);
            }
        }
        remove(pathname);
    }
    else
    {
        remove(pathname);
    }
        
}

