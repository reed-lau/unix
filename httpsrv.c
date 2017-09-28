#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

#include <threadpool.h>

int esocket(int domain,int type,int protocol);
int ebind(int sockfd,const struct sockaddr_in *addr,int len);
int elisten(int sockfd,int backlog);
int eaccept(int sockfd,struct sockaddr_in *addr,int *addrlen);
struct sockaddr_in mkaddr(char *ipbydot,int port);


typedef struct{

int fd;
char data[1024];
char *p;
	
}work_param;

void *task(void *arg)
{
	char buf[1024];
	work_param *p=(work_param*)(arg);
	
	
	printf("task[%d] <- %s\n",p->fd,p->data);
	
	sprintf(buf, "HTTP/1.1 200 OK\r\n");  
	write(p->fd,buf,strlen(buf));
	
//	sleep(rand()%3);
	
	close(p->fd);
	
	free(p->p);
}

int main(int argc,char *argv[])
{
	int cfd,sfd;
	char buf[1024];
	int  ret,i;
	char *p;

	struct sockaddr_in saddr,caddr;
	int    len;
	
	threadpool_t *pool;
	work_param   *param;
	
	srand(10086);
	
	sfd=esocket(AF_INET,SOCK_STREAM,0);
	
	saddr=mkaddr("127.0.0.1",8090);
	ebind(sfd,&saddr,sizeof(saddr));
	elisten(sfd,132);
	
	pool=threadpool_init(10,100);
	
	while (1)
	{
		len=sizeof(caddr);
		cfd=eaccept(sfd,&caddr,&len);
		if (cfd!=-1)
		{	
			memset(buf,0,1024);
			ret=read(cfd,buf,1024);
			for (i=0;i<1024;++i)
			{
				if (buf[i]=='\r'&&
				buf[i+1]=='\n'&&
				buf[i+2]=='\r'&&
				buf[i+3]=='\n')
				{
					p=buf+i+4;
					break;
				}
			}
			
			param=(work_param*)malloc(sizeof(work_param));
			param->fd=cfd;
			param->p=param;
			strncpy(param->data,p,1024);
			threadpool_addtask(pool,task,param);
			printf("here %s\n",p);
		}
		else
		{
			fprintf(stderr,"accept error\n");
		}
	}
	

	
	
	
	
}

int esocket(int domain,int type,int protocol)
{
	int fd;
	
	fd=socket(domain,type,protocol);
	
	if (fd==-1)
	{
		fprintf(stderr,"Error: socket Error\n");
		fprintf(stderr,"Error: %s\n",strerror(errno));
		exit(0);
	}
	return fd;
}

int ebind(int sockfd,const struct sockaddr_in *addr,int len)
{
	int ret;
	ret=bind(sockfd,(struct sockaddr*)(addr),(socklen_t)(len));
	if (ret==-1)
	{
		fprintf(stderr,"Error: bind Error\n");
		fprintf(stderr,"Error: %s\n",strerror(errno));
		exit(0);
	}
	return ret;
}

int elisten(int sockfd,int backlog)
{
	int ret;
	ret=listen(sockfd,backlog);
	if (ret==-1)
	{
		fprintf(stderr,"Error: listen Error\n");
		fprintf(stderr,"Error: %s\n",strerror(errno));
		exit(0);
	}
	return ret;
}

int eaccept(int sockfd,struct sockaddr_in *addr,int *addrlen)
{
	int ret;
	socklen_t len;
	len=*addrlen;
	ret=accept(sockfd,(struct sockaddr*)(addr),&len);
	*addrlen=len;
	if (ret==-1)
	{
		fprintf(stderr,"Error: accept Error\n");
		fprintf(stderr,"Error: %s\n",strerror(errno));
		exit(0);
	}
	return ret;
}

struct sockaddr_in mkaddr(char *ipbydot,int port)
{
	struct in_addr in;
	struct sockaddr_in addr;
	
	inet_pton(AF_INET,ipbydot,&in);
	
	addr.sin_family=AF_INET;
	addr.sin_addr=in;
	addr.sin_port=htons(port);
	
	return addr;
}

