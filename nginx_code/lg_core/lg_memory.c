
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include "lg_memory.h"

void *lg_memory_share_malloc(int size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
}

pthread_mutex_t *lg_memory_share_mutex_init()
{

    pthread_mutex_t *mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (!mutex) return NULL;

    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mattr);
    return mutex;

}


