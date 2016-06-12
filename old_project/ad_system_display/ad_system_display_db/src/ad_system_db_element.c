
#include "lg_db.h"
#include "lg_string.h"
#include "ad_system_display_db.h"
#include <stdarg.h>

ad_system_db_t *ad_system_db_element_create()
{

    ad_system_db_t *db = malloc(sizeof(ad_system_db_t));
    if (!db) return NULL;

    // create
    if (!(db->info_region_img = lg_db_create("info_region_img")))
	return NULL;

    if (!(db->info_region_txt = lg_db_create("info_region_txt")))
	return NULL;

    if (!(db->info_disease_img = lg_db_create("info_disease_img")))
	return NULL;

    if (!(db->info_disease_txt = lg_db_create("info_disease_txt")))
	return NULL;

    if (!(db->dict_place = lg_db_create("dict_place")))
	return NULL;

    if (!(db->dict_dept = lg_db_create("dict_dept")))
	return NULL;

    if (!(db->dict_disease = lg_db_create("dict_disease")))
	return NULL;

    if (!(db->dict_region = lg_db_create("dict_region")))
	return NULL;

    // add
    if (ad_system_db_load_info_region_img(db->info_region_img))
	return NULL;

    if (ad_system_db_load_info_region_txt(db->info_region_txt))
	return NULL;

    if (ad_system_db_load_dict_place(db->dict_place))
	return NULL;

    if (ad_system_db_load_dict_region(db->dict_region))
	return NULL;

    if (ad_system_db_load_dict_disease(db->dict_disease))
	return NULL;

    if (ad_system_db_load_dict_dept(db->dict_dept))
	return NULL;

    return db;
};

void ad_system_db_element_data_free(lg_list_t *m_list, void *m_data)
{
    if (m_data) free(m_data);
}

void ad_system_db_element_free(ad_system_db_t *db)
{

    if (!db) return;

    lg_db_free(db->info_region_img, ad_system_db_element_data_free);
    lg_db_free(db->info_region_txt, ad_system_db_element_data_free);
    lg_db_free(db->info_disease_img, ad_system_db_element_data_free);
    lg_db_free(db->info_disease_txt, ad_system_db_element_data_free);

    lg_db_free(db->dict_place, ad_system_db_element_data_free);
    lg_db_free(db->dict_dept, ad_system_db_element_data_free);
    lg_db_free(db->dict_disease, ad_system_db_element_data_free);
    lg_db_free(db->dict_region, ad_system_db_element_data_free);

    free(db);
}

int ad_system_db_element_add_kv(lg_db_t *db, char *key, char *buffer, size_t size)
{

    if (!(key && *key && buffer && size))
	return -1;

    lg_db_node_t *node = lg_db_string_node_add(db, key);
    if (node) {

	node->m_size = size;
	node->m_data = malloc(size);
	memcpy(node->m_data, buffer, size);
	return 0;
    }

    return -1;
}

int ad_system_db_element_add_list(lg_db_t *db,
	int region_type,
	char *region_code_array,
	int disease_type,
	char *disease_code_array,
	double cpc_price,
	double cpm_price, 
	char *buffer,
	size_t size)
{

    if (!(region_code_array 
		&& *region_code_array
		&& disease_code_array 
		&& *disease_code_array))
	return -1;

    char region_code[128], *region_save_ptr = NULL;
    char disease_code[128], *disease_save_ptr = NULL;
    while (lg_string_strtok(region_code_array, region_code, &region_save_ptr, ',') && *region_code) {

	lg_db_node_t *region_node = lg_db_string_node_add(db, region_code);
	if (region_node) {

	    while (lg_string_strtok(disease_code_array, disease_code, &disease_save_ptr, ',') && *disease_code) {

		lg_db_node_t *disease_node = lg_db_string_node_child_add(region_node, disease_code);
		if (disease_node) {

		    ad_system_info_t *info = malloc(sizeof(ad_system_info_t));
		    if (info) {

			info->size = size;
			info->buffer = malloc(size);
			info->cpc_price = cpc_price;
			info->cpm_price = cpm_price;
			if (info->buffer) {

			    memcpy(info->buffer, buffer, size);
			    lg_list_add(&disease_node->m_list, &info->next);

			} else {

			    free(info);
			}
		    }
		}

		disease_code_array = NULL;
	    }
	}

	region_code_array = NULL;
    }

    return 0;
}

size_t __ad_system_db_element_find(lg_db_t *db, char **buffer, va_list args)
{

    *buffer = NULL;
    char *key = va_arg(args, char *);
    lg_db_node_t *node = lg_db_string_find(db, key);
    fprintf(stdout, "[find][%s][%s][%p]\n", db->db_name, key, node);

    while(key && node) {

	key = va_arg(args, char *);
	if (key) {

	    node = lg_db_string_child_find(node, key);
	    fprintf(stdout, "[find][%s][%s][%p]\n", db->db_name, key, node);
	}
    }

    if (!node) return 0;

    *buffer = (char *)node->m_data;
    return node->m_size;
}

size_t ad_system_db_element_find(ad_system_db_t *db, char **buffer, ...)
{

    va_list args;
    lg_db_t *db_array[] = {
	db->dict_region,
	db->dict_disease,
	db->dict_dept,
	NULL
    };

    lg_db_t **p = NULL;
    for (p = db_array; p && *p; p++) {

	va_start(args, buffer);
	size_t size = __ad_system_db_element_find(*p, buffer, args);
	va_end(args);

	if (*buffer && size)
	    return size;
    }

    *buffer = NOT_DATA_STR;
    return NOT_DATA_SIZE;
}

size_t ad_system_db_element_find_data(lg_db_t *db, char **buffer, ...)
{

    va_list args;
    va_start(args, buffer);
    *buffer = NULL;

    char *key = va_arg(args, char *);
    lg_db_node_t *node = lg_db_string_find(db, key);
    fprintf(stdout, "[find][%s][%s][%p]\n", db->db_name, key, node);

    while(key && node) {

	key = va_arg(args, char *);
	if (key) {

	    node = lg_db_string_child_find(node, key);
	    fprintf(stdout, "[find][%s][%s][%p]\n", db->db_name, key, node);
	}
    }

    va_end(args);
    if (!node) {

	*buffer = NOT_DATA_STR;
	return NOT_DATA_SIZE;
    }

    *buffer = (char *)node->m_data;
    return node->m_size;
}

lg_list_t *ad_system_db_element_find_list(lg_db_t *db, ...)
{

    va_list args;
    va_start(args, db);

    char *key = va_arg(args, char *);
    lg_db_node_t *node = lg_db_string_find(db, key);
    fprintf(stdout, "[find][%s][%s][%p]\n", db->db_name, key, node);

    while(key && node) {

	key = va_arg(args, char *);
	if (key) {

	    node = lg_db_string_child_find(node, key);
	    fprintf(stdout, "[find][%s][%s][%p]\n", db->db_name, key, node);
	}
    }

    va_end(args);
    if (!node) return NULL;
    return &node->m_list;
}



