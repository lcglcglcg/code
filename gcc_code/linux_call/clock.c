#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define	MAX	10000000

int main(int argc, char **argv)
{

    clock_t start = clock();

    size_t i;
    char arr[MAX];

    srand(0);
    for (i = 0; i != MAX; ++i)arr[i] = rand();

    printf("The %s used %lfs!\n", "app", (double)(clock() - start) / CLOCKS_PER_SEC);

    return 0;
}
