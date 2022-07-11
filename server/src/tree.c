#include "../include/thread_pool.h"

int dfs_print(char *path, int width)
{
    DIR *dirp = opendir(path);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *pdirent;

    while (pdirent = readdir(dirp))
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }
        for (int i = 0; i < width; ++i)
        {
            printf("-");
        }
        printf("%s\n", pdirent->d_name);
        if (pdirent->d_type == 4)
        {
            char newPath[1024] = {0};
            sprintf(newPath, "%s%s%s", path, "/", pdirent->d_name);
            dfs_print(newPath, width + 4);
        }
    }
    closedir(dirp);
    return 0;
}

int mytree(char *pathname)
{
    dfs_print(pathname, 0);

    return 0;
}

