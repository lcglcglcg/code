
#include <stdio.h>
#include <stdlib.h>
#include "license_15talk.h"

int main(int argc, char *argv[])
{

    char buffer[4096] = {0};
    license_15talk_drivers_cpuid(buffer);
    license_15talk_drivers_mac_addr(&buffer[35]);
    license_15talk_drivers_licence("./License", &buffer[52]);
    fprintf(stdout, "%s\n", buffer);
    return 0;
}


