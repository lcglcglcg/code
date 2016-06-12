
#include <stdio.h>
#include <uuid/uuid.h>

void uuid_demo(void)
{

    uuid_t uu;
    uuid_generate(uu);

    int i;
    for(i = 0; i < 16; i++)
	printf("%02X", uu[i]);

    printf("\n");
}

int main(int argc, char *argv[])
{

    int i;
    for (i = 0; i < 1000; i++)
    uuid_demo();
    return 0;
}


