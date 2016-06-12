
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lg_config_file.h"

void lg_config_file_add(lg_list_t *head, char *name, char *value)
{

    lg_config_file_t *node = NULL;
    if (!(node = (lg_config_file_t *)malloc(sizeof(lg_config_file_t))))
	return;

    //fprintf(stdout, "[%s][%s]\n", name, value);

    strcpy(node->name, name);
    strcpy(node->value, value);
    lg_list_add(head, &node->list);
}

char *lg_config_file_find(lg_list_t *head, const char *name)
{

    lg_list_t *p = NULL;
    lg_list_for_each(p, head) {

	lg_config_file_t *node = (lg_config_file_t *)p;
	if (strcmp(node->name, name) == 0)
	    return node->value;
    }

    return NULL;
}

void lg_config_file_parse_r(char *str)
{
    for(;*str; str++) {

	if (*str == '\r' || *str == '\n') {

	    *str = 0;
	    return;
	}
    }
}

void lg_config_file_parse(lg_list_t *head, char *buffer)
{

    if (!isalnum(*buffer))
	return;

    char *str = NULL;
    if (!(str = strstr(buffer, "=")))
	return;

    *str++ = 0;
    lg_config_file_parse_r(str);
    lg_config_file_add(head, buffer, str);
}

int lg_config_file_load(lg_list_t *head, const char *path)
{

    lg_list_init(head);

    FILE *fp = NULL;
    if (!(fp = fopen(path, "r")))
	return -1;

    char buffer[4096];
    memset(buffer, 0, 4096);
    while(fgets(buffer, 4096, fp)) {

	if (buffer[4000])
	    return -1;

	lg_config_file_parse(head, buffer);
	memset(buffer, 0, 4096);
    }

    fclose(fp);
    return 0;
}


