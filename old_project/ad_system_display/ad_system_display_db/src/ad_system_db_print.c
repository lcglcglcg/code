
#include "ad_system_display_db.h"

void demo_buffer_print(char *buffer, size_t size)
{

    size_t i;
    char *str = buffer;
    for (i = 0; i < size; i++) {

	if (buffer[i] == CH_COMM) {

	    buffer[i] = 0;
	    fprintf(stdout, "==>[%s]\n", str); 
	    str = &buffer[i + 1];

	} else if (buffer[i] == CH_ROW) {

	    buffer[i] = 0;
	    fprintf(stdout, "==>[%s]\n", str); 
	    fprintf(stdout, "=======================\n");
	    str = &buffer[i + 1];
	}
    }
}

int demo_print(ad_system_db_t *db)
{

    size_t size = 0;
    char *buffer = NULL;
    lg_list_t *p, *head = NULL;

    // GetInfoImg 1001500000 1020004000
    head = ad_system_db_element_find_list(db->info_region_img, "1001500000", "1020004000", NULL);
    lg_list_for_each(p, head) {

	ad_system_info_t *info = (ad_system_info_t *)p;
	demo_buffer_print(info->buffer, info->size);
    }

    // GetInfoTxt 1001500000 1020004000
    head = ad_system_db_element_find_list(db->info_region_txt, "1001500000", "1020004000", NULL);
    lg_list_for_each(p, head) {

	ad_system_info_t *info = (ad_system_info_t *)p;
	demo_buffer_print(info->buffer, info->size);
    }

    // GetPlace TI2OJ65M4AGD2H
    size = ad_system_db_element_find(db, &buffer, "TI2OJ65M4AGD2H", NULL);
    demo_buffer_print(buffer, size);

    // GetCode nanke
    size = ad_system_db_element_find(db, &buffer, "nanke", NULL);
    demo_buffer_print(buffer, size);

    // GetCode jjxyz
    size = ad_system_db_element_find(db, &buffer, "jjxyz", NULL);
    demo_buffer_print(buffer, size);

    // GetCode beijing
    ad_system_db_element_find(db, &buffer, "beijing", NULL);
    demo_buffer_print(buffer, size);
    return 0;
}


