#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <time.h>

size_t current_time()
{
    size_t t;
#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
    static first = 1;
    static LARGE_INTEGER freq;
    LARGE_INTEGER tic;
    if (first)
    {
        if (QueryPerformanceFrequency(&freq))
        {
            first = 0;
        }
        else
        {
            t = 0;
            return t;
        }
    }

    QueryPerformanceCounter(&tic);
    t = tic.QuadPart*1E9/freq.QuadPart;
#else
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    t = tp.tv_nsec + tp.tv_sec*1E9;
#endif
    return t;
}

int main(int argc,char *argv[])
{
	int sfd;
	char *pipename="liuwei";
	struct sockaddr_un addr;
	size_t len;
	int   cfd;
	int ret;
	size_t tic,toc;

	char buf[1024];


	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	
	if (sfd<0)
	{
		fprintf(stderr,"socket error");
		return 0;
	}
	
	fcntl(sfd,F_SETFL,O_NONBLOCK);
	
	unlink(pipename);

	memset(&addr,0,sizeof(addr));

	addr.sun_family=AF_UNIX;
	sprintf(addr.sun_path,"%s",pipename);

	len=offsetof(struct sockaddr_un,sun_path)+strlen(addr.sun_path)+1;

	bind(sfd,(struct sockaddr*)(&addr),len);

	listen(sfd,1);

	while(1)
	{

		cfd=accept(sfd,NULL,0);
	
		if (cfd==-1 && errno==EWOULDBLOCK )
		{
			sleep(1);
			continue;
		}
		printf("[conn in]\n");

		fcntl(cfd,F_SETFL,O_NONBLOCK);

		while(1)
		{
			memset(buf,0,1024);
			tic=current_time();

			ret=read(cfd,buf,1024);
			if (ret==-1 && errno==EWOULDBLOCK)
			{
			//	sleep(1);
				continue;
			}
			else if (ret==0)
			{
				printf("[conn out]\n");
				break;
			}
			toc=current_time();
			printf("[recv:] %lfs\n",(double)((toc-tic)*1.0e-9));
		
			errno=0;	
			ret=write(cfd,buf,strlen(buf));
			printf("write ret=%d errno=%d str=%s\n",ret,errno,strerror(errno));
			if (ret==-1&&errno==EWOULDBLOCK)
			{
				continue;
			}
			else if (ret==0)
			{
				printf("[conn out\n]\n");
				break;
			}
		}
	}

}


