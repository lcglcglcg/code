
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libxl.h"
#include "lg_list.h"
#include "lg_config_file.h"

typedef struct {

    lg_list_t next;

    int row;
    int col;
    char *data;
    char *path;

} xls_file_t;

int xls_file_load(lg_list_t *head, char *path, char *title_name)
{

    lg_list_init(head);
    BookHandle book = NULL;
    if (!(book = xlCreateXMLBook()))
	return -1;

    if(!xlBookLoad(book, path))
	return -1;

    SheetHandle sheet = NULL;
    if (!(sheet = xlBookGetSheet(book, 0)))
	return -1;

    int title_name_col = -1;
    int row, row_max = xlSheetLastRow(sheet);
    int col, col_max = xlSheetLastCol(sheet);

    for (col = 0; col < col_max; col++) {

	char *str = NULL;
	if (!(str = (char *)xlSheetReadStr(sheet, 0, col, 0)))
	    return -1;

	if (strcmp(str, title_name) == 0)
	    title_name_col = col;
    }

    if (title_name_col == -1)
	return -2;

    for (row = 1; row < row_max; row++) {

	char *str = NULL;
	if (!(str = (char *)xlSheetReadStr(sheet, row, title_name_col, 0)))
	    return -1;

	xls_file_t *node = malloc(sizeof(xls_file_t));
	if (!node) return -3;
	memset(node, 0, sizeof(xls_file_t));

	node->data = malloc(strlen(str));
	if (!node->data) return -3;
	strcpy(node->data, str);

	node->row = row;
	node->col = title_name_col;
	lg_list_add(head, &node->next);
    }

    xlBookRelease(book);
    return 0;
}

void file_name_cmp(lg_list_t *head, char *file_name, char *file_path)
{    

    lg_list_t *p = NULL;
    lg_list_for_each(p, head) {

	xls_file_t *node = (xls_file_t *)p;
	if (strstr(file_name, node->data)) {

	    if (!node->path) {

		node->path = malloc(strlen(file_path) + 1);
		if (node->path) strcpy(node->path, file_path);
		return;
	    }
	}
    }
}

int find_dir_file(lg_list_t *head, char *path)
{

    DIR *pDir = opendir(path);
    if (pDir == NULL)
	return -1;

    struct dirent *ent = NULL;
    while (NULL != (ent = readdir(pDir))) {

	if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
	    continue;

	file_name_cmp(head, ent->d_name, path);
	if (ent->d_type == 16 || ent->d_type == 4) {

	    char _path[strlen(ent->d_name) + strlen(path) + 2];
	    sprintf(_path, "%s/%s", path, ent->d_name);
	    find_dir_file(head, _path);
	}
    }

    return 0;
}

int out_display(lg_list_t *head, char *path)
{

    BookHandle book = NULL;
    if (!(book = xlCreateXMLBook()))
	return -1;

    if(!xlBookLoad(book, path))
	return -1;

    SheetHandle sheet = NULL;
    if (!(sheet = xlBookGetSheet(book, 0)))
	return -1;

    FormatHandle format = xlBookAddFormat(book, NULL);

    xlFormatSetAlignH(format, ALIGNH_CENTER);
    xlFormatSetBorder(format, BORDERSTYLE_MEDIUMDASHDOTDOT);
    xlFormatSetBorderColor(format, COLOR_RED);

    lg_list_t *p = NULL;
    lg_list_for_each(p, head) {

	xls_file_t *node = (xls_file_t *)p;
	if (node->path) {

	    fprintf(stdout, "row=%d::value=%s::path=%s\n",
		    node->row,
		    node->data,
		    node->path);

	    xlSheetWriteStr(sheet, node->row, node->col, node->data, format);
	}
    }

    xlBookSave(book, path);
    xlBookRelease(book);
    return 0;
}

int main()
{

    lg_list_t config_head;
    lg_config_file_load(&config_head, "./config.ini");
    char *XLSX_FILE = lg_config_file_find(&config_head, "XLSX_FILE");
    char *TITLE_NAME = lg_config_file_find(&config_head, "TITLE_NAME");
    char *DIR_PATH = lg_config_file_find(&config_head, "DIR_PATH");
    if (!XLSX_FILE || !TITLE_NAME || !DIR_PATH) {

	fprintf(stdout, "load config.ini err!");
	getchar();
	return -1;
    }

    lg_list_t xls_head;
    if (xls_file_load(&xls_head, XLSX_FILE, TITLE_NAME)) {

	fprintf(stdout, "load execl file err!");
	getchar();
	return -1;
    }

    find_dir_file(&xls_head, DIR_PATH);
    out_display(&xls_head, XLSX_FILE);

    fprintf(stdout, "Match completed!");
    getchar();
    return 0;
}


