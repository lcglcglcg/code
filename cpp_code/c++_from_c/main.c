#ifdef __cplusplus
extern "C" { 
#endif
    int add(int a, int b);
    int sub(int a, int b);
#ifdef __cplusplus
}
#endif

#include <stdio.h>

int main(int argc, char **argv)
{
    printf("%d\n", add(3, 2));
    printf("%d\n", sub(3, 2));
    return 0;
}
