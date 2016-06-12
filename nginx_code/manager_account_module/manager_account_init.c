
#include "lg_config_file.h"
#include "manager_account_sql.h"
#include "manager_account_redis.h"
#include "manager_account_module.h"

int manager_account_init()
{

    srand(time(NULL));

    lg_list_t config_file_head;
    if (lg_config_file_load(&config_file_head, "/etc/manager_account_init.conf")) {

	fprintf(stdout, "load config file err\n");
	return -1;
    }

    if (manager_account_sql_init(&config_file_head)) {

	fprintf(stdout, "sql init err\n");
	return -1;
    }

    if (manager_account_redis_init(&config_file_head)) {

	fprintf(stdout, "redis init err\n");
	return -1;
    }

    return 0;
}

