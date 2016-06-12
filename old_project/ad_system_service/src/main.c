
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lg_log_file.h"
#include "lg_config_file.h"
#include "ad_system_session.h"

extern int ad_system_oci_init();
extern int ad_system_mongo_init();
extern int ad_system_global_var_init();
extern int ad_system_sql_init(lg_list_t *config_file_head);

extern int ad_system_network_run();
extern int ad_system_upload_keyword_run();
extern int ad_system_upload_image_run();

void *manager_ad_system_network_run(void *arg)
{

    ad_system_network_run();
    pthread_exit(NULL);
}

void *manager_ad_system_upload_keyword_run(void *arg)
{

    ad_system_upload_keyword_run();
    pthread_exit(NULL);
}

void *manager_ad_system_upload_image_run(void *arg)
{

    ad_system_upload_image_run();
    pthread_exit(NULL);
}

int main()
{

    if (fork() > 0) exit(1);

    lg_list_t config_file_head;
    lg_config_file_load(&config_file_head, "/etc/ad_system_config.conf");
    ad_system_oci_init(&config_file_head);
    ad_system_sql_init(&config_file_head);
    ad_system_mongo_init(&config_file_head);
    ad_system_global_var_init();

    extern int mongo_main();
    //return mongo_main();


    pthread_t network_run_tid;
    pthread_create(&network_run_tid, NULL, manager_ad_system_network_run, NULL);

    pthread_t upload_keyword_run_tid;
    pthread_create(&upload_keyword_run_tid, NULL, manager_ad_system_upload_keyword_run, NULL);

    pthread_t upload_image_run_tid;
    pthread_create(&upload_image_run_tid, NULL, manager_ad_system_upload_image_run, NULL);

    pthread_join(network_run_tid, NULL);
    pthread_join(upload_keyword_run_tid, NULL);
    pthread_join(upload_image_run_tid, NULL);

    return 0;
}



