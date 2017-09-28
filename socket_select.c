#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <helper.h>

int main(int argc,char *argv[])
{
	int sfd;
	struct sockaddr_in addr,caddr;
	size_t len;
	socklen_t clen;
	int   cfd;
	int ret;
	size_t tic,toc;

	char buf[1024];
	int i,nfds,nact;

	fd_set rfds;
	
	int *watch,nwatch;

	sfd=esocket(AF_INET,SOCK_STREAM,0);

	fcntl(sfd,F_SETFL,O_NONBLOCK);
	addr=mkaddr("10.132.128.220",8090);

	len=sizeof(addr);
	ebind(sfd,&addr,len);

	elisten(sfd,1);

	/* watched fd */
	watch=(int*)malloc(sizeof(int)*1024);
	memset(watch,0,sizeof(int)*1024);

	/* Zero out all fds */
	watch[sfd]=1;

	while (1)
	{
		/* Set fd_set */
		nfds=0;
		FD_ZERO(&rfds);
		for (i=0;i<1024;++i)
		{
			if ( watch[i] )
			{
				nfds=i;
				FD_SET(i,&rfds);
			}
		}

		nact=select(nfds+1,&rfds,NULL,NULL,NULL);
		
		if (nact>=0)
		{
			for (i=0;i<=nfds;++i)
			{
				/* New Connection or Read Avaliable */
				if ( FD_ISSET(i,&rfds) )
				{
					/* New Connection */
					if (i==sfd)
					{
						printf("[new connection]\n");
						clen=sizeof(caddr);
						/* Accept new connection  */
						cfd=accept(sfd,(struct sockaddr*)(&caddr),&clen);
						fcntl(cfd,F_SETFL,O_NONBLOCK);

						/* Add to watch list */
						watch[cfd]=1;	
					}
					else /* Read Avaliable */
					{
						printf("[write ]\n");
						while (1)
						{
							memset(buf,0,1024);
							ret=read(i,buf,1000);
							/* read complete */
							if (ret==-1 && ( errno==EWOULDBLOCK ||errno==EAGAIN ))
							{
								break;
							}
							else if (ret==0) /* remote closed  */
							{
								watch[i]=0;
								printf("[closed]\n");
								break;
							}
							printf("==%s\n",buf);
							write(i,buf,strlen(buf));
							break;
						}
					}
				}
			}
		}
		else
		{
			fprintf(stderr,"Error: select error\n");
			fprintf(stderr,"       %s\n",strerror(errno));
			exit(0);
		}
	}

	return 0;
}
