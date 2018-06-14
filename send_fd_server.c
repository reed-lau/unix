#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int send_fd(int sock, int fd)  
{  
    iovec iov[1];  
    char c = 0;  
    iov[0].iov_base = &c;  
    iov[0].iov_len  = 1;  
      
    int cmsgsize = CMSG_LEN(sizeof(int));  
    cmsghdr* cmptr = (cmsghdr*)malloc(cmsgsize);  
    if(cmptr == NULL){  
        printf("[send_fd] init cmptr error\n");  
        exit(1);  
    }  
    cmptr->cmsg_level = SOL_SOCKET;  
    cmptr->cmsg_type = SCM_RIGHTS; // we are sending fd.  
    cmptr->cmsg_len = cmsgsize;  
  
    msghdr msg;  
    msg.msg_iov = iov;  
    msg.msg_iovlen = 1;  
    msg.msg_name = NULL;  
    msg.msg_namelen = 0;  
    msg.msg_control = cmptr;  
    msg.msg_controllen = cmsgsize;  
    *(int *)CMSG_DATA(cmptr) = fd;  
      
    int ret = sendmsg(sock, &msg, 0);  
    free(cmptr);  
    if (ret == -1){  
        printf("[send_fd] sendmsg error\n");  
        exit(1);  
    }  
} 

int main()
{

    int fd;
    int ret;
    struct sockaddr_un addr, addr1;
    socklen_t len;
    int i;

    char str[1024];

    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if ( fd == -1)
        perror("socket error");

    addr.sun_family = AF_UNIX;
    sprintf(addr.sun_path, "xliuwei");

    addr.sun_path[0]=0;

    len = offsetof(struct sockaddr_un, sun_path) + 7;

    printf("len=%d\n", (long long)(len) );

    ret = bind(fd, (struct sockaddr*)(&addr), len); 


    if ( ret == -1)
        perror("bind fail\n");

    ret = listen(fd, 4);

    if ( ret == -1)
       perror("listen fail\n");

    len = sizeof(addr1);
    ret = accept(fd, (struct sockaddr*)(&addr1), &len); 

    if ( ret == -1 )
        perror("accept fail\n");

    int ff = open("aaaaaa", O_CREAT|O_TRUNC|O_RDWR, 00777);

    if ( ff <0 )
       perror(" file open error\n");

    send_fd(ret, ff); 

    return 0;

    i=0;
    while(1)
    {
        printf("ok\n");
        sprintf(str, "i=%d", i);
        // write(ret , str, strlen(str));  
        //
        sleep(1);
        i++;
    }

}
