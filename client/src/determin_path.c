#include "../include/myfunc.h"

int determin_path(char *pathname, char *arg)
{
    if (strncmp(arg, "/", 1) == 0)
    {
        memset(pathname, 0, sizeof(pathname));
        strcpy(pathname, arg);
    }
    else if (strncmp(arg, "~", 1) == 0)
    {
        memset(pathname, 0, sizeof(pathname));
        sprintf(pathname, "%s%s", "/home/simon", arg+1);
    }
    else //相对路径
    {
        sprintf(pathname, "%s%s%s", pathname, "/", arg);
    }

}
