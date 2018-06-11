#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

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

    i=0;
    while(1)
    {
        printf("ok\n");
        sprintf(str, "i=%d", i);
        write(ret , str, strlen(str));  
        sleep(1);
        i++;
    }

}
