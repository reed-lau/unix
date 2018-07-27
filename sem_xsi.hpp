#ifndef ZORO_XSI_SEMAPHARE_HPP
#define ZORO_XSI_SEMAPHARE_HPP

#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

namespace zoro {
namespace interprocess {
namespace xsi {

inline bool sem_create(::key_t key, int nsem, int &semid)
{
    int id, perm;

    union semun {
        int         val;
        ::semid_ds *buf;
        ushort     *array;
    } semctl_arg;

    semid = -1;
    perm = 0666;

    if (key == IPC_PRIVATE)
        return false; //not intended for private semaphores

    else if (key == (::key_t) -1)
        return false; //probably an ftok() error by caller

    if ((id = ::semget(key, nsem+2, (perm & 0x01FF) | IPC_CREAT)) < 0)
        return false;   //permission problem or tables full

    // semaphore set
    // [0] : lock, used for creation atomic
    // [1] : process-count, process attach it
    // [2...]: semaphore used for user

    // set all the values to zero
    for (int i=0; i<nsem; ++i)
    {
        semctl_arg.val = 0;
        if (::semctl(id, i+2, SETVAL, semctl_arg) < 0)
            return false;
    }

    semid = id;
    return true;
}

inline bool sem_open(::key_t key, int nsem, int &semid)
{
    int id;
    semid = -1;

    if (key == IPC_PRIVATE)
        return false; //not intended for private semaphores

    else if (key == (::key_t) -1)
        return false; //probably an ftok() error by caller

    if ((id = ::semget(key, nsem+2, 0)) < 0)
        return false;   //permission problem or tables full

    // semaphore set
    // [0] : lock, used for creation atomic
    // [1] : process-count, process attach it
    // [2...]: semaphore used for user

    semid = id;
    return true;
}

inline bool sem_inc(int id, int ipos)
{
    ::sembuf op = {
        0, +1, SEM_UNDO
    }; 

    op.sem_num = ipos+2;

    if (::semop(id, &op, 1) < 0) 
        return false;
    return true;
}

inline bool sem_desc(int id, int ipos)
{
    ::sembuf op = {
        0, -1, SEM_UNDO
    }; 

    op.sem_num = ipos+2;

    if (::semop(id, &op, 1) < 0) 
        return false;
    return true;
}

inline int sem_get(int id, int ipos)
{
    return ::semctl(id, ipos+2, GETVAL);
}

}  // namespace xsi
}  // namespace interprocess
}  // namespace zoro

#endi
