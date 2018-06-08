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
#define FILE_NAME_MAX_SIZE 512

//����
int connectTCP(const char *host, const char *port){
	
	int clientfd;
	struct sockaddr_in clientaddr;
	bzero(&clientaddr,sizeof(clientaddr));	

	clientaddr.sin_family=AF_INET;
	clientaddr.sin_addr.s_addr=htons(INADDR_ANY);
	clientaddr.sin_port=htons(0);
	
	clientfd=socket(AF_INET,SOCK_STREAM,0);
	
	if(clientfd<0)	
	{
		perror("socket");
		exit(1);
	}

	if(bind(clientfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0)
	{
		perror("bind");
		exit(1);
	}

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
	if(connect(clientfd,(struct sockaddr*)&svraddr,svraddrlen)<0)	
	{
		perror("connect");
		exit(1);
	}
	return clientfd;
}

//������Ϣ
void recvTCP(int clientfd){
	//recv file imformation
	char buff[BUFFSIZE];
	char filename[FILE_NAME_MAX_SIZE];
	int count;
	bzero(buff,BUFFSIZE);

	count=recv(clientfd,buff,BUFFSIZE,0);
	if(count<0)
	{
		perror("recv");
		exit(1);
	}
	strncpy(filename,buff,strlen(buff)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buff));

	printf("Preparing recv file : %s  \n",filename);	

	
	//recv file
	FILE *fd=fopen(filename,"wb+");
	if(NULL==fd)
	{
		perror("open");
		exit(1);
	}
	bzero(buff,BUFFSIZE);

	int length=0;
	while(length=recv(clientfd,buff,BUFFSIZE,0))
	{
		if(length<0)
		{
			perror("recv");
			exit(1);
		}
		int writelen=fwrite(buff,sizeof(char),length,fd);
		if(writelen<length)
		{
			perror("write");
			exit(1);
		}
		bzero(buff,BUFFSIZE);
	}
	printf("Receieved file:%s  finished!\n",filename);
	fclose(fd);
}


int main(int argc, char **argv[])
{
	int clientfd;
	
	//����
	clientfd=connectTCP("127.0.0.1",PORT);
	//������Ϣ
	recvTCP(clientfd);
	//�ر�
	close(clientfd);
	return 0;
}