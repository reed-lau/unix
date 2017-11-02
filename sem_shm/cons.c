#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
    int n=10;
    sem_t *sem_blank,*sem_fill,*sem_mutex;
    int shm; 

    int *p,i1,i2,i;

    sem_blank=sem_open("blank",O_RDWR );
    sem_fill =sem_open("fill" ,O_RDWR );
    sem_mutex=sem_open("mutex",O_RDWR );
    
    
    shm=shm_open("/shm-bounded-buffer", O_RDWR , 0600 );
    ftruncate(shm,sizeof(int)*(n+2));

    p=mmap(NULL,sizeof(int)*(n+2),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);
    
    /* consume  */
    while(1)
    {
        sem_wait(sem_fill);
        sem_wait(sem_mutex);
        
        /* put & get position */
        i1=p[n];
        i2=p[n+1];
        
        /* get */
        i=p[i2];
        //printf("get %d\n",i);
        /* next get position */     
        p[n+1]=(p[n+1]+1)%n;   

        sem_post(sem_mutex);
        sem_post(sem_blank);
        
        //sleep(1); 

    }
}
