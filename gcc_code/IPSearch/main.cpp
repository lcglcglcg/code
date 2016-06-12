
#include "IPSearch.h"

int main(int argc, char *argv[])
{

    char db_path[] = "ip.dat";
    char ip[] = "202.97.225.123";
    char buffer[1024] = {0};

    IPSearch ip_search(db_path);
    ip_search.get_location_utf8(ip, buffer, 1024);
    fprintf(stdout, "%s\n", buffer);

    return 0;
}



