#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{


    char temp[] = "XXXXXX";
    int fd = mkstemp(temp);
    unlink(temp);
    ftruncate(fd, 1024);
    void *p = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    memset(p, 0, 1024);

    int i;
    for(i = 0; i < 10; i++) {
	if (fork() == 0) {
	    char *s = p + i;
	    *s = '0' + i;
	    exit(1);
	}
    }

    char *s = p + i;
    *s = '\n';
    write(1, p, 1024);
    return 0;
}
