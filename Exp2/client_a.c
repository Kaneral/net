#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 6000
#define LISTENQ 20
#define BUFFSIZE 4096
#define STRING_MAX_SIZE 512

int connectTCP(const char *host, const char *port){
        int clientstr;
        struct sockaddr_in clientaddr;
        bzero(&clientaddr,sizeof(clientaddr));

        //由于client和server都在本地机上，所以将设置回送地址127.0.0.1
        clientaddr.sin_family=AF_INET;
        clientaddr.sin_addr.s_addr=htons(INADDR_ANY);
        clientaddr.sin_port=htons(0);

        //Create socket
        //（初始化服务器端的socket：0成功；-1失败）
        clientstr=socket(AF_INET,SOCK_STREAM,0);

        if(clientstr<0)
        {
                perror("socket");
                exit(1);
        }

        //bind （绑定地址：0成功；-1失败）
        if(bind(clientstr,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0)
        {
                perror("bind");
                exit(1);
        }

        //连接服务器
        struct sockaddr_in svraddr;
        bzero(&svraddr,sizeof(svraddr));
        if(inet_aton(host,&svraddr.sin_addr)==0)
        {
                perror("inet_aton");
                exit(1);
        }
        svraddr.sin_family=AF_INET;
        svraddr.sin_port=htons(port);

        socklen_t svraddrlen=sizeof(svraddr);
        if(connect(clientstr,(struct sockaddr*)&svraddr,svraddrlen)<0)
        {
                perror("connect");
                exit(1);
        }
        return clientstr;
}



 //收发数据
void recvTCP(int clientstr)
{
        char buff[BUFFSIZE];
        char string[STRING_MAX_SIZE];
        int count;
        bzero(buff,BUFFSIZE);

        count=recv(clientstr,buff,BUFFSIZE,0);
        if(count<0)
        {
                perror("recv");
                exit(1);
        }
        strncpy(string,buff,strlen(buff)>STRING_MAX_SIZE?STRING_MAX_SIZE:strlen(buff));

        printf("Receieved string:%s  finished!\n",string);
}

int main(int argc, char **argv[])
{
        int clientstr;

        //建立连接
        clientstr=connectTCP("127.0.0.1",PORT);
        //接收信息
        recvTCP(clientstr);
        //关闭
        close(clientstr);
        return 0;
}