//
// Created by 梦 on 2019-03-15.
//

#include "TcpClient.h"
#include "unp.h"
#include <thread>
#include <vector>

int start_client_connect(const char * ip,int port){
   int cli = socket(AF_INET, SOCK_STREAM, 0);

   struct sockaddr_in serv;
   bzero(&serv,sizeof(serv));
   serv.sin_port = htons(port);
   serv.sin_family = AF_INET;
   inet_pton(AF_INET,ip,&serv.sin_addr);

   int retval = connect(cli, reinterpret_cast<sockaddr*>(&serv), sizeof(serv));
   if(retval < 0){
      fprintf(stderr,"client: connect %s:%d fail\n",ip,port);
      return -1;
   }

   return cli;
}

int handle_recv_data(int sockfd, const char * buf, int size){
   assert(size > 0);
   sleep(1);
   return write(sockfd, buf, size);
}

void handle_connect(int clifd, struct timeval tv){
   char buff[MAX_BUFF] = {0};
   fd_set readset;
   struct timeval tmptv;
   do{
      FD_ZERO(&readset);
      FD_SET(clifd,&readset);
      memcpy(&tmptv, &tv, sizeof(tv));

      int retset = select(clifd+1,&readset,NULL,NULL,&tmptv);
      if(0 == retset){
          printf("client select time out \n");
          continue;
      }

      if(-1 == retset){
          printf("client select error :%s\n",strerror(errno));
          break;
      }

      if(FD_ISSET(clifd,&readset)){
          int ret = read(clifd, buff, MAX_BUFF);
          if (ret <= 0) {
              fprintf(stderr, "client: server is closed \n");
              break;
          } else{
              ret = handle_recv_data(clifd, buff, strlen(buff)+1);
              printf("client send data length:%d \n",ret);
          }
      }
   }while(1);
    close(clifd);
    FD_CLR(clifd,&readset);
}

void testServer(int cli_count)
{
    assert(cli_count > 0);

    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    int ret = inet_pton(AF_INET,IPPADDR,&serv_addr.sin_addr);
    if(-1 == ret){
       printf("testServer IP transport error \n");
       return;
    }

    int reuse = -1;
    setsockopt(serv_fd,SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse));

    if(-1 == bind(serv_fd,(sockaddr*)&serv_addr, sizeof(serv_addr))){
       printf("bind server address error \n");
    }

    listen(serv_fd,15);
    printf("server@start test server %s:%d \n", IPPADDR, PORT);

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    while(cli_count--) {
        int cli_fd = accept(serv_fd, (sockaddr *) &cli_addr, &cli_len);

        printf("server@get client %s:%u \n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        char buff[4] = {0};
        int read_len = 0;
        read_len = read(cli_fd, buff, sizeof(buff));

        if(0 == read_len)
            printf("server@client close connect \n");
        else
            printf("server@get buffer form client: %s len:%d\n", buff,read_len);

        printf("server@read again return %d \n",read_len);

        write(cli_fd, buff, read_len + 1);
        sleep(1);
        close(cli_fd);
    }
    printf("server no client to wait and ready to close \n");
    close(serv_fd);
}

void create_client_connect(int index){
    int cli = 0;
   cli = start_client_connect(IPPADDR,PORT);


    printf("create client %d\n",index);
   char buf[MAX_BUFF]= {0};
   sprintf(buf,"client data %d ",index);
   handle_recv_data(cli,buf,strlen(buf)+1);

   struct timeval tv;
   tv.tv_usec = 0;
   tv.tv_sec = 10;
   handle_connect(cli,tv);
}

int main(){
    int count = 10;
    int sid = fork();

    if(0 == sid) {
        testServer(count);
        exit(0);
    }else {
        sleep(2);
        int cpid = 0;
        for (int i = 0; i < count; i++) {
            cpid = fork();
            if (0 == cpid) {
                create_client_connect(getpid());
                exit(0);
            }
        }

        int status = 0;
        while (1) {
            sid = wait(&status);
            if (-1 == sid)
                break;
            fprintf(status!=0?stderr:stdout,"sid %d is exit %d\n", sid,status);
            if(status)
                fprintf(stderr, "get last error :%s\n", strerror(errno));
        }

    }
}

