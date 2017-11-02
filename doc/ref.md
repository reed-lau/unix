http://man7.org/training/download/posix_shm_slides.pdf

http://menehune.opt.wfu.edu/Kokua/More_SGI/007-2478-008/sgi_html/ch03.html#id81035

http://menehune.opt.wfu.edu/Kokua/More_SGI/007-2478-008/sgi_html/ch01.html#id54272

https://www.ibm.com/developerworks/cn/aix/library/au-ipc/index.html

   


| System V      | Posix.1-2001 .1-2008  | Cool  |
| ------------- |:---------------------:| -----:|
| semget        | sem_open              |       |
| semop         | sem_wait              | lock  |
| semctl        | sem_post              | unlock|
|               | sem_close             |       |
|               | sem_unlink            |       |
