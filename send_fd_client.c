#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/uio.h> 
#include <string.h> 
#include <stdlib.h> 

int recv_fd(int sock)  
{  
    int cmsgsize = CMSG_LEN(sizeof(int));  
    struct cmsghdr* cmptr = (struct cmsghdr*)malloc(cmsgsize);  
      
    char buf[32]; // the max buf in msg.  
    struct iovec iov[1];  
    iov[0].iov_base = buf;  
    iov[0].iov_len = sizeof(buf);  
      
    msghdr msg;  
    msg.msg_iov = iov;  
    msg.msg_iovlen  = 1;  
    msg.msg_name = NULL;  
    msg.msg_namelen = 0;  
    msg.msg_control = cmptr;  
    msg.msg_controllen = cmsgsize;  
      
    int ret = recvmsg(sock, &msg, 0);  
    free(cmptr);  
    if (ret == -1) {  
        printf("[recv_fd] recvmsg error\n");  
        exit(1);  
    }  
      
    int fd = *(int *)CMSG_DATA(cmptr);  
    return fd;  
}  

int main()
{
	int fd;
	int ret;
	struct sockaddr_un addr;
	socklen_t len;

	int n;

	char str[1024];

	fd = socket(AF_UNIX, SOCK_STREAM, 0);

	if ( fd == -1)
		perror("socket error");

	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path, "xliuwei");

	addr.sun_path[0]=0;

	len = offsetof(struct sockaddr_un, sun_path) + 7;
	ret = connect(fd, (struct sockaddr*)(&addr), len); 

	unlink("liuwei");

	if ( ret == -1)
		perror("connect fail\n");

        int ff = recv_fd(fd);

        if ( ff < 0 )
           perror("recv fd error\n");
  
        sprintf(str,"i am very happy");

        write(ff, str, strlen(str));

        close(ff);

        return 0;

	while(1)
	{
		n=read(fd, str, 10); 

		str[n]=0;
		printf("recv: %s\n", str);

	}

}
