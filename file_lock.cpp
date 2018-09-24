#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include <mutex>
#include <thread>

#include <iostream>
#include <chrono>

using namespace std::chrono;

class mutex_locker
{
  public:
      mutex_locker() = default;
      int lock()
      {
          mutex_.lock();
          return 0;
      }

      int unlock()
      {
          mutex_.unlock();
          return 0;
      }
  private:
      std::mutex mutex_;
};

class locker
{
    public:
        locker()
        {
            fd_ = open("lock.lock", O_CREAT | O_EXCL, 0777);
            fd_ = open("lock.lock", O_RDWR );
        }
    int lock()
    {
        int ret;
        ret = flock(fd_, LOCK_EX);
        return 0;
    }

    int unlock()
    {
        int ret;
        ret = flock(fd_, LOCK_UN);
        return ret;
    }
private:
    int fd_;
};

int start = 0;
long long sum = 0;


    mutex_locker l;
void foo()
{
    while(!start)
        ;


    auto tic = std::chrono::high_resolution_clock::now();
    for (int i=0; i<=100000; ++i)
    {
	    l.lock();

            sum += i;
  
	    l.unlock();   
    }
    auto toc = std::chrono::high_resolution_clock::now();

    auto dur = duration_cast<milliseconds>(toc - tic);
    std::cout<< dur.count()<< std::endl;
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
