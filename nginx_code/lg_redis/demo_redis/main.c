
#include "hiredis.h"

int main()
{

    redisContext *c = redisConnect("127.0.0.1", 6379);
    redisCommand(c, "select 3");
    redisCommand(c, "set lcg 1");
    redisCommand(c, "rpush lcglcg 1");

    return 0;
}



