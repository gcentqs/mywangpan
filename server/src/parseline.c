#include "../include/thread_pool.h"

int parseline(char *cmdline, char **argv)
{
    int argc = 0;
    
    char *p = cmdline;
    cmdline[strlen(cmdline)] = ' ';//在结尾添加空格
    while (*p && (*p == ' '))   //忽略最开始的空格
    {
        p++;
    }

    char *delim;
    while (delim = strchr(p, ' '))
    {
        argv[argc++] = p;
        *delim = '\0'; 
        p = delim + 1;
        while (*p && (*p == ' '))//跳过空格
        {
            p++;
        }
    }

    return argc;
}

