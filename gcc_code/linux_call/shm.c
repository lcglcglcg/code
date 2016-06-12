
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "lg_list.h"

int _main(int argc, char** argv)
{

    key_t key = ftok("./shm", 0x03);
    printf("key=%d\n",key) ;

    int shm_id = shmget(key, 4096, IPC_CREAT | IPC_EXCL | 0600);
    printf("shm_id=%d\n", shm_id) ;

    void *p = shmat(shm_id, NULL, 0);
    sprintf(p, "verylcg");

    shmdt(p);
    return 0;
}

int main(int argc, char** argv)
{

    key_t key = ftok("./shm", 0x03);
    printf("key=%d\n",key) ;

    int shm_id = shmget(key, 0, 0);
    printf("shm_id=%d\n", shm_id) ;

    char *p = shmat(shm_id, NULL, 0);
    fprintf(stdout, "%p::%s\n", p, p);

    int ret = shmdt(p);
    fprintf(stdout, "ret=%d\n", ret);
    return 0 ;
}


