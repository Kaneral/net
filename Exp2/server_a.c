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

int main(int argc, char **argv[])
{
        //Input the string
        char string[STRING_MAX_SIZE];
        bzero(string,STRING_MAX_SIZE);
        printf("Please input the string you wana to send:");
        scanf("%s",&string);
        getchar();

        int sockstr,connstr;
        struct sockaddr_in svraddr,clientaddr;
        bzero(&svraddr,sizeof(svraddr));

        //����client��server���ڱ��ػ��ϣ����Խ����û��͵�ַ127.0.0.1
        svraddr.sin_family=AF_INET;
        svraddr.sin_addr.s_addr=htonl(INADDR_ANY);
        svraddr.sin_port=htons(PORT);

        //Create socket
        //����ʼ���������˵�socket��0�ɹ���-1ʧ�ܣ�
        sockstr=socket(AF_INET,SOCK_STREAM,0);

        if(sockstr<0)
        {
                perror("socket");
                exit(1);
        }

        //bind ���󶨵�ַ��0�ɹ���-1ʧ�ܣ�
        if(bind(sockstr,(struct sockaddr*)&svraddr,sizeof(svraddr))<0)
        {
                perror("bind");
                exit(1);
        }

        //listen�������˿ڣ�0�ɹ���-1ʧ�ܣ�
        if(listen(sockstr,LISTENQ)<0)
        {
                perror("listen");
                exit(1);
        }


        while(1)
        {
                socklen_t length=sizeof(clientaddr);
	//�ȴ�����
                connstr=accept(sockstr,(struct sockaddr*)&clientaddr,&length);
                if(connstr<0)
                {
                        perror("connect");
                        exit(1);
                }

                //�շ�����
                char buff[BUFFSIZE];
                int count;
                bzero(buff,BUFFSIZE);
                strncpy(buff,string,strlen(string)>STRING_MAX_SIZE?STRING_MAX_SIZE:strlen(string));
                count=send(connstr,buff,BUFFSIZE,0);
                if(count<0)
                {
                        perror("Send string");
                        exit(1);
                }
                printf("String :%s \n Finished!\n",string);

        }
        close(connstr);
        close(sockstr);
        return 0;
}

