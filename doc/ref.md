http://man7.org/training/download/posix_shm_slides.pdf

http://menehune.opt.wfu.edu/Kokua/More_SGI/007-2478-008/sgi_html/ch03.html#id81035

http://menehune.opt.wfu.edu/Kokua/More_SGI/007-2478-008/sgi_html/ch01.html#id54272

https://www.ibm.com/developerworks/cn/aix/library/au-ipc/index.html

http://www.cnblogs.com/whatbeg/p/4435286.html   

https://stackoverflow.com/questions/2584678/how-do-i-synchronize-access-to-shared-memory-in-lynxos-posix

http://lib.csdn.net/article/operatingsystem/40462

lock free queue

https://coolshell.cn/articles/8239.html

http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.53.8674&rep=rep1&type=pdf

https://www.ibm.com/developerworks/cn/aix/library/au-multithreaded_structures2/index.html

https://stackoverflow.com/questions/2489908/is-it-possible-to-store-pointers-in-shared-memory-without-using-offsets


### Semaphore 
| System V      | Posix.1-2001 .1-2008  | Cool  |
| ------------- |:---------------------:| -----:|
| semget        | sem_open              |       |
| semop         | sem_wait              | lock  |
| semctl        | sem_post              | unlock|
|               | sem_close             |       |
|               | sem_unlink            |       |


| Shared Memory | Synchronization |              |
|:-------------:|:---------------:|-------------:|
|  Posix        |   Semaphore     |              |
|  System V     |   Mutex & Cond  |              |


Memory Slab Nginx

https://mp.weixin.qq.com/s/OkeCJSYyIUR01sF6ZT9Xmg?utm_medium=hao.caibaojian.com&utm_source=hao.caibaojian.com

