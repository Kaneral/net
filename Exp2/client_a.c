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

        //����client��server���ڱ��ػ��ϣ����Խ����û��͵�ַ127.0.0.1
        clientaddr.sin_family=AF_INET;
        clientaddr.sin_addr.s_addr=htons(INADDR_ANY);
        clientaddr.sin_port=htons(0);

        //Create socket
        //����ʼ���������˵�socket��0�ɹ���-1ʧ�ܣ�
        clientstr=socket(AF_INET,SOCK_STREAM,0);

        if(clientstr<0)
        {
                perror("socket");
                exit(1);
        }

        //bind ���󶨵�ַ��0�ɹ���-1ʧ�ܣ�
        if(bind(clientstr,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0)
        {
                perror("bind");
                exit(1);
        }

        //���ӷ�����
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



 //�շ�����
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

        //��������
        clientstr=connectTCP("127.0.0.1",PORT);
        //������Ϣ
        recvTCP(clientstr);
        //�ر�
        close(clientstr);
        return 0;
}