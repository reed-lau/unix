#include <helper.h>

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

int econnect(int sockfd,struct sockaddr_in *addr)
{
	int ret;
	int len=sizeof(struct sockaddr_in);
	ret=connect(sockfd,(struct sockaddr*)(addr),len);
	if (ret==-1)
	{
		fprintf(stderr,"Error: connect Error\n");
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

