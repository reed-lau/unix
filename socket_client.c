#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <helper.h>

int main(int argc,char *argv[])
{
	int sfd;
	char *pipename="liuwei";
	struct sockaddr_in addr;
	size_t len;
	int   cfd;
	int ret;

	char buf[1024];

	sfd=esocket(AF_INET,SOCK_STREAM,0);
	
//	fcntl(sfd,F_SETFL,O_NONBLOCK);

	addr=mkaddr("10.132.128.220",8090);

	econnect(sfd,&addr);
	
	
	while(1)
	{
		gets(buf,1024);	
		ret=write(sfd,buf,strlen(buf));
		
		while(1)
		{
			ret=read(sfd,buf,strlen(buf));
			if (ret==-1 )
			{
				continue;
			}
			else if (ret==0)
			{
				break;
			}
			
			printf(":%s\n",buf);
			break;
		}	
	}
}


