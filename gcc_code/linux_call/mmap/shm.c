#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int shmid;
    char *shmadd;
    struct shmid_ds shmbuf;

    if((shmid=shmget(IPC_PRIVATE,1000,0666))<0)
    {perror("shmget");exit(1);}
    else printf("created shared-memory: %d\n",shmid);
    system("ipcs -m");

    if((shmadd=shmat(shmid,0,0))<(char *)0)
    { perror("shmat");exit(1); }
    else printf("attached shared-memory\n");
    system("ipcs -m");

    if((shmdt(shmadd))<0) //禁止本进程再使用该共享内存区
    {perror("shmdt");exit(1);}
    else printf("deleted shared-memory\n");
    system("ipcs -m");
    if(shmctl(shmid,IPC_RMID,&shmbuf)<0)
    {perror("shmctl");exit(1);}
    system("ipcs -m");

    return 0;
}
shmget
int shmget(key_t key, size_t size, int flag);
key: 标识符的规则
size:共享存储段的字节数
flag:读写的权限
返回值：成功返回共享存储的id，失败返回-1
shmat
void *shmat(int shmid, const void *addr, int flag);
shmid：共享存储的id
addr：一般为0，表示连接到由内核选择的第一个可用地址上，否则，如果flag没有指定SHM_RND，则连接到addr所指定的地址上，如果flag为SHM_RND，则地址取整
flag：如前所述，一般为0
返回值：如果成功，返回共享存储段地址，出错返回-1
shmdt
int shmdt(void *addr);
addr:共享存储段的地址，以前调用shmat时的返回值
shmdt将使相关shmid_ds结构中的shm_nattch计数器值减1
    shmctl
int shmctl(int shmid,int cmd,struct shmid_ds *buf)
    shmid:共享存储段的id
    cmd：一些命令，有：IPC_STAT,IPC_RMID,SHM_LOCK,SHM_UNLOCK
    程序说明：
    结合我一些函数的说明，这个程序应该就很容易懂了，另外需要补充的是，查看共享内存段的命令式：ipcs -m，删除共享内存段的命令是：ipcrm -m shmid,请注意，共享内存不会随着程序结束而自动消除，要么调用shmctl删除，要么自己用手敲命令去删除，否则永远留在系统中
    另看
    ipc_write.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

    typedef struct
{
    char name;
    int age;
}people;

int main(int argc,char **argv)
{
    int shm_id,i;
    key_t key;
    char temp_char;
    people *p_map;

    shm_id=shmget(IPC_PRIVATE,4096,IPC_CREAT);
    if(shm_id<0)
    {perror("shmget error");return;}
    p_map=(people*)shmat(shm_id,NULL,0);
    temp_char='a';
    for(i=0;i<10;i++)
    {
	(*(p_map+i)).name=temp_char;
	(*(p_map+i)).age=20+i;    
	temp_char+=1;
    }
    if(shmdt(p_map)<0)
	perror("detach error");

    return 0;
}
ipc_read.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct
{
    char name;
    int age;
}people;
int main(int argc,char **argv)
{
    int shm_id,i;
    key_t key;
    people *p_map;
    struct shmid_ds shmbuf;

    shm_id=393216;//393216是ipcs -m查看到的shmid
    p_map=(people*)shmat(shm_id,NULL,0);
    for(i=0;i<10;i++)
    {
	printf("name----------%c\n",(*(p_map+i)).name);
	printf("age------------%d\n",(*(p_map+i)).age);
    }
    if(shmdt(p_map)<0)
	perror("shmdt error");
    if(shmctl(shm_id,IPC_RMID,&shmbuf)<0)
	perror("shmctl error");

    return 0;
}
ipc_write.c实现向共享内存里面写，ipc_read.c实现从共享内存里面读
不过在ipc_write.c的shmget的第一个参数是IPC_PRIVATE，所以分配的key的值是
由系统产生的，并且没有任何标志独特性的key_t的值。那么ipc_read.c是如何知道shm_id的呢?我这个是自己手工查ipcs -m得来的，那么编程中如何实现呢？
1.建立具有非常鲜明特征，独特的key值
2.通过信号量，消息队列或者管道(FIFO)传输这个shm_id
shm_id也有人用，不过稍微麻烦一点，下面介绍用第一种方法
shm_ftok_write.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct
{
    char name;
    int age;
}people;

int main(int argc,char **argv)
{
    int shm_id,i;
    key_t key;
    char temp_char;
    people *p_map;
    char *name="/dev/shm/myshm1";

    key = ftok(name,0);
    printf("key=%d\n",key);
    if(key==-1)
	perror("ftok error");
    shm_id=shmget(IPC_PRIVATE,4096,IPC_CREAT);
    printf("shm_id=%d\n",shm_id);
    if(shm_id==-1)
    {perror("shmget error");return;}
    p_map=(people*)shmat(shm_id,NULL,0);
    temp_char='a';
    for(i=0;i<10;i++)
    {
	(*(p_map+i)).name=temp_char;
	temp_char+=1;
	(*(p_map+i)).age=20+i;
    }
    if(shmdt(p_map)==-1)
	perror("detach error");

    return 0;
}
ipc_ftok_read.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct
{
    char name;
    int age;
}people;
int main(int argc,char **argv)
{
    int shm_id,i;
    key_t key;
    people *p_map;
    char *name="/dev/shm/myshm1";

    key = ftok(name,0);
    if(key==-1)
	perror("ftok error");
    shm_id=shmget(key,4096,IPC_CREAT);
    printf("shm_id=%d\n",shm_id);
    if(shm_id==-1)
    {perror("shmget error");return;}
    p_map=(people*)shmat(shm_id,NULL,0);
    p_map=(people*)shmat(393216,NULL,0);
    for(i=0;i<10;i++)
    {
	printf("name----------%c\n",(*(p_map+i)).name);
	printf("age-----------%d\n",(*(p_map+i)).age);
    }
    if(shmdt(p_map)==-1)
	perror("detach error");

    return 0;
}
这里用的是独特鲜明的key值
执行程序之前，先创建"/dev/shm/myshm1"文件
最好是写成自己的执行文件的目录，如"/home/nsl/myprogram/ipc_ftok_write"这样就不会存在文件不存在的情况了
    ftok
key_t ftok( char * fname, int id )
    frame：文件的全路径，要求文件存在且进程可访问
    id：第几个共享内存
    返回值：返回key值
    注明：系统V共享内存与mmap相比，就是系统V共享内存从来不把数据真正写到磁盘文件中去，而mmap机制在munmap的时候将数据写到磁盘文件
    发现原来系统V共享内存也需要文件的支撑
    当然，共享内存没有这么容易，还有大量的同步要做
