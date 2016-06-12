
#include "memory.h"

void memory_buffer_row_add(memory_buffer_storage_t *storage)
{

    if (storage && storage->head == NULL) {

	return;

    } else if (storage->row_last == NULL) {

	storage->row_last = storage->row_head;

    } else {

	memory_buffer_t *node = NULL;
	memory_buffer_t *node_last = NULL;

	if (storage->row_head != storage->row_last) {

	    for (node = storage->row_head, 
		    node_last = storage->row_last; 
		    node;
		    node = node->col_next, 
		    node_last = node_last->col_next) {

		node_last->row_next = node;
	    }
	}

	storage->row_last = storage->row_head;
	storage->row_head = NULL;
	storage->col_last = NULL;
    }
}

void memory_buffer_col_add(memory_buffer_storage_t *storage,
	memory_buffer_t *node)
{

    if (storage->head == NULL)
	storage->head = node;

    if (storage->row_last == NULL)
	storage->row_last = node;

    if (storage->row_head == NULL) 
	storage->row_head = node;

    if (storage->col_last == NULL) {

	storage->col_last = node;

    } else {

	storage->col_last->col_next = node;
	storage->col_last = node;
    }

    node->row_head = storage->row_head;
}

void memory_buffer_head_to_storage(memory_buffer_t *head, memory_buffer_storage_t *storage)
{

    memory_buffer_t *node = NULL;

    if (head) {

	storage->head = head;
	storage->row_head = NULL;
	storage->row_last = NULL;
	storage->col_last = NULL;

	for (node = head; node; node = node->row_next)
	    storage->row_last = node;
    }
}

void memory_buffer_row_insert(memory_buffer_t *row, memory_buffer_t *new_row)
{

    memory_buffer_t *node = NULL;
    memory_buffer_t *new_node = NULL;

    for (node = row, new_node = new_row;
	    node && new_node;
	    node = node->col_next,
	    new_node = new_node->col_next) {

	new_node->row_next = node->row_next;
	node->row_next = new_node;
    }
}

void memory_buffer_free(memory_buffer_t *head)
{

    memory_buffer_t *row = head;
    memory_buffer_t *node = NULL;

    while(row) {

	node = row;
	row = row->row_next;
	free(node);
    }
}

void memory_buffer_print(memory_buffer_t *head, char *mode)
{

    if (mode == NULL)
	return;

    if (strcmp("row", mode) == 0) {

	memory_buffer_t *row = NULL;
	memory_buffer_t *node = NULL;

	for (row = head; row; row = row->row_next) {

	    for (node = row; node; node = node->col_next) {

		if (node->col_next) {

		    if (node->buffer[0])
			fprintf(stdout, "%s,", node->buffer);
		    else
			fprintf(stdout, ",");

		} else {

		    if (node->buffer[0])
			fprintf(stdout, "%s", node->buffer);
		}
	    }

	    fprintf(stdout, "\n");
	}

    } else if (strcmp("col", mode) == 0) {

	memory_buffer_t *col = NULL;
	memory_buffer_t *node = NULL;

	for (col = head; col; col = col->col_next) {

	    for (node = col; node; node = node->row_next) {

		if (node->buffer[0])
		    fprintf(stdout, "%s,", node->buffer);
		else
		    fprintf(stdout, "null,");
	    }

	    fprintf(stdout, "\n");
	}
    }


    fprintf(stdout, "\n");
}

void memory_buffer_print_r(char *path, memory_buffer_t *head, char *mode)
{

    FILE *fp = NULL;

    if (mode == NULL)
	return;

    if (!(fp = fopen(path, "wd")))
	return;

    if (strcmp("row", mode) == 0) {

	memory_buffer_t *row = NULL;
	memory_buffer_t *node = NULL;

	for (row = head; row; row = row->row_next) {

	    for (node = row; node; node = node->col_next) {

		if (node->col_next) {

		    if (node->buffer[0])
			fprintf(fp, "%s,", node->buffer);
		    else
			fprintf(fp, ",");

		} else {

		    if (node->buffer[0])
			fprintf(fp, "%s", node->buffer);
		}
	    }

	    fprintf(fp, "\n");
	}

    } else if (strcmp("col", mode) == 0) {

	memory_buffer_t *col = NULL;
	memory_buffer_t *node = NULL;

	for (col = head; col; col = col->col_next) {

	    for (node = col; node; node = node->row_next) {

		if (node->buffer[0])
		    fprintf(fp, "%s,", node->buffer);
		else
		    fprintf(fp, "null,");
	    }

	    fprintf(fp, "\n");
	}
    }


    fprintf(fp, "\n");
    fclose(fp);
}

#if 0
int memory_demo(int argc, char *argv[]) 
{

    memory_buffer_t *node = NULL;
    memory_buffer_storage_t _storage;
    memory_buffer_storage_t *storage = &_storage;
    memset(&_storage, 0, sizeof(memory_buffer_storage_t));

    memory_buffer_t *temp_node = NULL;
    memory_buffer_storage_t _storage_temp;
    memset(&_storage_temp, 0, sizeof(memory_buffer_storage_t));

    for (int i = 0; i < 5; i++) {

	for (int j = 0; j < 5; j++) {

	    node = malloc(sizeof(memory_buffer_t));
	    memset(node, 0, sizeof(memory_buffer_t));
	    sprintf(node->buffer, "%02d", i * 5 + j + 1);
	    memory_buffer_col_add(storage, node);
	}

	memory_buffer_row_add(storage);
	if (i == 2)
	    temp_node = node->row_head;
    }

    memory_buffer_print(storage->head, "row");
    memory_buffer_print(storage->head, "col");


    storage = &_storage_temp;
    for (int j = 0; j < 5; j++) {

	node = malloc(sizeof(memory_buffer_t));
	memset(node, 0, sizeof(memory_buffer_t));
	sprintf(node->buffer, "%02d", j);
	memory_buffer_col_add(storage, node);
    }

    memory_buffer_row_insert(temp_node, storage->row_head);
    storage = &_storage;
    memory_buffer_print(storage->head, "row");
    memory_buffer_print(storage->head, "col");
    return 0;
}

#endif


