
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lg_list.h"

void lg_filter_keyword_add(lg_list_t *head, char *keyword)
{

    int keyword_len = strlen(keyword) - 1;
    if (keyword_len <= 1) return;

    lg_list_t *p = malloc(sizeof(lg_list_t) + keyword_len);
    if (!p) return;

    p->str = (char *)&p[1];//(char *)(((char *)p) + sizeof(lg_list_t));
    memcpy(p->str, keyword, keyword_len);
    p->str[keyword_len - 1] = 0;
    p->size = keyword_len;
    lg_list_add(head, p);
}

int lg_filter_keyword_load(lg_list_t *head, const char *path)
{

    lg_list_init(head);

    FILE *fp = NULL;
    if (!(fp = fopen(path, "r")))
	return -1;

    char buffer[4096] = {0};
    while(fgets(buffer, 4096, fp)) {

	if (buffer[4000])
	    return -1;

	lg_filter_keyword_add(head, buffer);
	memset(buffer, 0, 4096);
    }

    fclose(fp);
    return 0;
}


