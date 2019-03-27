//
// Created by 梦 on 2019-03-15.
//

#ifndef CLIONPROJECTS_UNP_H
#define CLIONPROJECTS_UNP_H

#include <sys/socket.h> /* basic socket definitions */
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/event.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPPADDR "127.0.0.1"
#define PORT 8787
#define MAX_BUFF 1024
#define LISTENQ 5

typedef struct tagfdSetUnit{
    fd_set _rfd;
    fd_set rfd;
}fdUnit;

typedef struct tagServContext{
    fdUnit cli_fd;
    int max_fd;
    int serv_fd;
}ServContext;

#define max(a,b) ((a) > (b)? (a):(b))

#endif //CLIONPROJECTS_UNP_H
