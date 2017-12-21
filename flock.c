#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv)
{
    int fd;
    pid_t pid;

    char *name="lock.lock";

    pid=getpid();

    /* 1. create lock file */
    fd=open(name,O_CREAT|O_EXCL,0600);
    if (fd>0)
    {
       printf("pid:%u create lock file ok\n",pid);
       close(fd);
    }

    /* 2. open lock file by on process one time */
    
    fd=open(name,O_RDWR);

    flock(fd,LOCK_EX);
    printf("lock pid=%u\n",pid);

    sleep(4);

    flock(fd,LOCK_UN);
    printf("unlock pid=%u\n",pid);

    close(fd);

}
