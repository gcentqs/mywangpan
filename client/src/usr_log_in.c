#include "../include/myfunc.h"
#define USR_NAME_SIZE 20
#define USR_PWD_SIZE 20
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)

int set_disp_mode(int fd,int option)
{
    int err;
    struct termios term;
    if(tcgetattr(fd,&term)==-1){
        perror("Cannot get the attribution of the terminal");
        return 1;

    }
    if(option)
        term.c_lflag |= ECHOFLAGS;
    else
        term.c_lflag &=~ECHOFLAGS;
    err=tcsetattr(fd,TCSAFLUSH,&term);
    if(err==-1 && err==EINTR){
        perror("Cannot set the attribution of the terminal");
        return 1;

    }
    return 0;
}


int usr_log_in(int server_fd)
{
    train_t train;
    memset(&train, 0, sizeof(train));

    char usr_name[USR_NAME_SIZE] = {0};
    char usr_pwd[USR_PWD_SIZE] = {0};

    int log_flag;
    int data_len;

    while (1)
    {
        memset(usr_name, 0, USR_NAME_SIZE);
        memset(usr_pwd, 0, USR_PWD_SIZE);

        printf("please enter your name\n");
        scanf("%s", usr_name);
        train.length = strlen(usr_name);
        strcpy(train.buf, usr_name);
        send(server_fd, &train, sizeof(int)+train.length, 0);

        printf("please enter your passwd\n");
        set_disp_mode(STDIN_FILENO, 0);
        scanf("%s", usr_pwd);
        train.length = strlen(usr_pwd);
        strcpy(train.buf, usr_pwd);
        send(server_fd, &train, sizeof(int)+train.length, 0);

        set_disp_mode(STDIN_FILENO, 1);

        recv(server_fd, &data_len, sizeof(int), 0);
        recv(server_fd, &log_flag, data_len, 0);
        if (1 == log_flag)  //登陆成功
        {
            printf("login successfully!\n\n");
            break;
        }
    }

    return 0;
}

