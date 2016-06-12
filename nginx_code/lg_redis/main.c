
#include <stdio.h>
#include "lg_redis.h"

int main()
{

    lg_redis_conn_t conn;
    if (lg_redis_init(&conn, "127.0.0.1", 6379)) return -1;

    lg_redis_add_key_value(&conn, "lcg", 3, "1234", 4);

    lg_redis_add_list_key_value(&conn, "lcglcg", 6, "1234", 4);



    return 0;
}




