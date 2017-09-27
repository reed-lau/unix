#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>

int main(int argc,char *argv[])
{
	int sfd;
	char *pipename="liuwei";
	struct sockaddr_un addr;
	size_t len;
	int   cfd;
	int ret;

	char buf[1024];

	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	
	if (sfd<0)
	{
		fprintf(stderr,"socket error");
		return 0;
	}
	
	fcntl(sfd,F_SETFL,O_NONBLOCK);
	

	memset(&addr,0,sizeof(addr));

	addr.sun_family=AF_UNIX;
	sprintf(addr.sun_path,"%s",pipename);

	len=offsetof(struct sockaddr_un,sun_path)+strlen(addr.sun_path)+1;

	ret=connect(sfd,(struct sockaddr*)(&addr),len);

	
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


