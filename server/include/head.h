#ifndef __HEAD_H__
#define __HEAD_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/uio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>
#include <pwd.h>
#include <grp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <setjmp.h>
#include <arpa/inet.h>
#include <aio.h>

#define ARGS_CHECK(argc,val) {if(argc!=val) \
    {printf("error args\n"); return -1;}}
#define ERROR_CHECK(ret,val,info) {if(ret == val) {perror(info); return -1;}}
#define THREAD_ERRORCHECK(ret,funcName) {if(0 != ret) {  printf("%s : %s \n",funcName,strerror(ret));} }

#endif
