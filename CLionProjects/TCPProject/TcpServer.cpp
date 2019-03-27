//
// Created by 梦 on 2019-03-15.
//

#include "TcpServer.h"
#include "unp.h"

int start_tcpserver(const char * ip,int port,ServContext * servCont){
    do{
        int sfd = socket(AF_INET,SOCK_STREAM, 0);

        if(-1 == sfd){
            break;
        }

        int reuse = 1;
        if(-1== setsockopt(sfd,SOL_SOCKET, SO_REUSEADDR,&reuse, sizeof(reuse)))
            break;

        struct sockaddr_in serv;
        bzero(&serv,sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_port = ntohs(port);
        inet_pton(AF_INET, ip, &serv.sin_addr);

        if(-1 == bind(sfd,reinterpret_cast<sockaddr*>(&serv), sizeof(serv)))
            break;

        FD_ZERO(&(servCont->cli_fd.rfd));

        listen(sfd,LISTENQ);

        servCont->serv_fd = sfd;
        servCont->max_fd = sfd;
        FD_SET(sfd,&servCont->cli_fd.rfd);

        return 0;
    }while(0);

    fprintf(stderr, "create socket error %s\n",strerror(errno));
    return -1;
}

int accept_cli(ServContext * servCont){
    assert(servCont && servCont->serv_fd);

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int cli_fd = -1;

    do{
        cli_fd = accept(servCont->serv_fd,reinterpret_cast<sockaddr*>(&cli_addr), &cli_len);
        if(-1 == cli_fd){
            if(EINTR == errno)
                continue;
            else
                break;
        }

        fprintf(stdout,"accept a new client: %s:%u\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));

//        if((servCont->max_fd +1)== sizeof(fd_set)){
//            perror("fdset is full \n");
//            break;
//        }

        FD_SET(cli_fd,&(servCont->cli_fd.rfd));
        servCont->max_fd = max(cli_fd,servCont->max_fd);
        return 0;
    }while(1);

    fprintf(stderr, "accept client error:%s\n",strerror(errno));
    return -1;
}

int handle_sockmsg(int fd,const char * msg_data,int msg_len){
    assert(msg_data && msg_len>0);
    assert(fd > 0);
    printf("send to client %d: %s \n",fd,msg_data);
    return write(fd, msg_data,msg_len);
}

int recv_client_msg(int cfd,fd_set * readfd){
    char buf[MAX_BUFF] = {0};
    int recv_len = 0;
    int ret = 0;

    recv_len = read(cfd, buf, MAX_BUFF);
    printf("get from client %d:%s \n",cfd, buf);
    if(recv_len > 0)
        ret = handle_sockmsg(cfd, buf,recv_len);
    FD_CLR(cfd, readfd);
    close(cfd);

    return ret;
}

int handle_client_proc(ServContext * servCont,struct timeval * tvp){

    struct timeval tv;
    do {
        memcpy(&tv,tvp,sizeof(tv));
        memcpy(&(servCont->cli_fd._rfd), &(servCont->cli_fd.rfd), sizeof(fd_set));

        int retval = select(servCont->max_fd + 1, &servCont->cli_fd._rfd, NULL, NULL, &tv);

        for(int j = 0; j <= servCont->max_fd; j++){
            if(FD_ISSET(j,&(servCont->cli_fd._rfd))){
                if(j == servCont->serv_fd)
                    accept_cli(servCont);
                else
                    recv_client_msg(j, &servCont->cli_fd.rfd);
            }
        }

    }while(1);
}

void clear_tcpserver(tagServContext * serv){
    assert(serv && serv->serv_fd > 0);
    close(serv->serv_fd);
}

int main(int argc, char *argv[])
{
    tagServContext serv;
    do {
        start_tcpserver(IPPADDR, PORT, &serv);

        if (serv.serv_fd < 0) {
            fprintf(stderr, "server create error \n");
            break;
        }
        else{
            printf("start server :%d sucess\n", PORT);
        }


        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        handle_client_proc(&serv, &tv);
    }while(0);

    clear_tcpserver(&serv);
}

