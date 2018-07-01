#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <thread>

int lock()
{
   int fd;
   int ret;

   fd = open("lock.lock", O_CREAT | O_EXCL, 0700);

   if ( fd != -1 )
       close(fd);

   fd = open("lock.lock", O_RDWR );
   if ( fd == -1 )
     return -1;

   ret = flock(fd, LOCK_EX);
   if ( ret == -1 )
      return -1;

   close(fd); 

   return 0;
}

int unlock()
{
   int fd;
   int ret;

   fd = open("lock.lock", O_RDWR );
   if ( fd == -1 )
     return -1;

   ret = flock(fd, LOCK_UN);
   if ( ret == -1 )
      return -1;

   close(fd); 
//   unlink("lock.lock");

   return 0;
}

int start = 0;
long long sum = 0;


void foo()
{
    while(!start);

    for (int i=0; i<=100000; ++i)
    {

	    lock();

            sum += i;
  
	    unlock();   

    }
}

int main(int argc,char *argv[])
{

     start = 0;

     std::thread t1(foo);
     std::thread t2(foo);

     start = 1;

     t1.join();
     t2.join();

     printf("sum=%lld\n", sum);
}
