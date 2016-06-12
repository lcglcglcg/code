
#include <unistd.h>
#include "License.h"

int main(int argc, char *argv[])
{

    int ret = License_packet_key_check("./License");
    fprintf(stdout, "ret=%d\n",ret);
    return 0;
}


