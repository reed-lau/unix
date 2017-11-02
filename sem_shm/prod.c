#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>

int main(int argc,char *argv[])
{
    int n=10;
    sem_t *sem_blank,*sem_fill,*sem_mutex;
    int shm; 

    int *p,i1,i2,i;

    sem_blank=sem_open("blank",O_CREAT | O_EXCL , 0600,n );
    sem_fill =sem_open("fill" ,O_CREAT | O_EXCL , 0600,0 );
    sem_mutex=sem_open("mutex",O_CREAT | O_EXCL , 0600,1 );
    
    
    shm=shm_open("/shm-bounded-buffer",O_CREAT | O_RDWR | O_EXCL, 0600 );
    ftruncate(shm,sizeof(int)*(n+2));

    p=mmap(NULL,sizeof(int)*(n+2),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);

    i=0;
    /* produce  */
    while(1)
    {
        sem_wait(sem_blank);
        sem_wait(sem_mutex);
        
        /* put & get position */
        i1=p[n];
        i2=p[n+1];
        
        /* put */
        p[i1]=i;
       // printf("buf[%d]=%d\n",i1,i); 
        /* next put position */     
        p[n]=(p[n]+1)%n;   

        sem_post(sem_mutex);
        sem_post(sem_fill);
        i++;
    }
}
