#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <func.h>
#include <termios.h>
#define ARGS_SIZE 20

typedef struct {
    int length;
    char buf[1024];
} train_t;

int determin_args(int, char (*)[ARGS_SIZE]);
int determin_path(char *, char *);
int parseline(char *, char **);
int puts_file(int, char *);
int gets_file(int, char *);
int usr_log_in(int);

#endif
