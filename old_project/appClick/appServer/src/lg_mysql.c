
#include "lg_mysql.h"

static pthread_mutex_t mysql_pool_mutex = PTHREAD_MUTEX_INITIALIZER;

lg_list_t *lg_mysql_pool_init(char *host, char *name, char *pass, char *base, int pool_max)
{

    lg_list_t *head = NULL;
    if (!(head = malloc(sizeof(lg_list_t))))
	return NULL;

    lg_mysql_pool *pool = NULL;
    if (!(pool = malloc(sizeof(lg_mysql_pool) * pool_max))) {

	free(head);
	return NULL;
    }

    lg_list_init(head);
    memset(pool, 0, (sizeof(lg_mysql_pool) * pool_max));

    int i;
    for (i = 0; i < pool_max; i++) {

	lg_mysql_pool *node = &pool[i];
	strcpy(node->host, host);
	strcpy(node->name, name);
	strcpy(node->pass, pass);
	strcpy(node->base, base);

	if (!mysql_init(&node->mysql)) {

	    free(head);
	    free(pool);
	    return NULL;
	}

	if (!mysql_real_connect(&node->mysql, host, name, pass, base, 0, 0, 0)) {

	    free(head);
	    free(pool);
	    return NULL;
	}

	lg_list_add(head, &node->node);
    }

    return head;
}

lg_mysql_pool *lg_mysql_pool_get(lg_list_t *head)
{

    lg_list_t *p = NULL;
    lg_mysql_pool *node = NULL;
    pthread_mutex_lock(&mysql_pool_mutex);

    lg_list_for_each(p, head) {

	node = rb_entry(p, lg_mysql_pool, node);
	if (node->status == 0) {

	    if (mysql_ping(&node->mysql) != 0) {

		mysql_close(&node->mysql);
		if (!mysql_real_connect(&node->mysql, 
			    node->host, 
			    node->name, 
			    node->pass, 
			    node->base, 0, 0, 0)) {

		    node->status = 2;
		    pthread_mutex_unlock(&mysql_pool_mutex); 
		    return NULL;
		}
	    }

	    node->status = 1;
	    pthread_mutex_unlock(&mysql_pool_mutex);
	    return node;
	}
    }

    pthread_mutex_unlock(&mysql_pool_mutex); 
    return NULL;
}

int demo_mysql_select(MYSQL *mysql)
{

    char sql_str[] = "select * from user_click";
    mysql_real_query(mysql, sql_str, strlen(sql_str));
    MYSQL_RES *res = mysql_store_result(mysql);

    MYSQL_ROW row = NULL;
    while((row = mysql_fetch_row(res))) {

	fprintf(stdout, "[%s][%s][%s][%s][%s]\n", row[0], row[1], row[2], row[3], row[4]);
    }

    mysql_free_result(res);
    return 0;
}

int demo_mysql_main(int argc, char *argv[])
{

    lg_list_t *head = NULL;
    head = lg_mysql_pool_init(DB_HOST, DB_USER, DB_PASS, DB_BASE, 100);

    lg_mysql_pool *pool = NULL;
    pool = lg_mysql_pool_get(head);
    pool = lg_mysql_pool_get(head);
    pool = lg_mysql_pool_get(head);

    demo_mysql_select(&pool->mysql);
    return 0;
}


