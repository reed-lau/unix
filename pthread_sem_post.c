#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>

int main(int argc,char *argv[])
{
	sem_t *sem;
	int ret;

	sem=sem_open("/liuwei",O_CREAT|O_RDWR,0666,0);
	if (sem==SEM_FAILED)
	{
		fprintf(stderr,"Error: sem_open error\n");
		fprintf(stderr,"       %s\n",strerror(errno));
		exit(0);
	}
	
	printf("before post\n");

	sem_post(sem);

	printf("after post\n");
sleep(5);
	printf("before wait\n");
	
	sem_wait(sem);

	printf("after wait\n");


	sem_close(sem);
	

}


int pthread_sem_init(sem_t *sem,char *fname,unsigned int value)
{
	sem_t *p;
//	p=sem_open(fname,);
}

