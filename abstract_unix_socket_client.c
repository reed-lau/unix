#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <unistd.h>

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

    // unlink("liuwei");

    if ( ret == -1)
        perror("connect fail\n");

    while(1)
    {
        n=read(fd, str, 10); 

        str[n]=0;
        printf("recv: %s\n", str);

    }

}
