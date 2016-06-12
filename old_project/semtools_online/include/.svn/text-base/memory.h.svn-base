
#ifndef	__MEMORY_H__
#define	__MEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef struct memory_buffer_t memory_buffer_t;
typedef struct memory_buffer_storage_t memory_buffer_storage_t;

struct memory_buffer_t {

    char buffer[256];
    memory_buffer_t *row_head;
    memory_buffer_t *row_next;
    memory_buffer_t *col_next;
};

struct memory_buffer_storage_t {

    memory_buffer_t *head;
    memory_buffer_t *row_head;
    memory_buffer_t *row_last;
    memory_buffer_t *col_last;
};

extern void memory_buffer_row_add(memory_buffer_storage_t *storage);
extern void memory_buffer_col_add(memory_buffer_storage_t *storage,
	memory_buffer_t *node);
extern void memory_buffer_row_insert(memory_buffer_t *row, 
	memory_buffer_t *new_row);
extern void memory_buffer_head_to_storage(memory_buffer_t *head, 
	memory_buffer_storage_t *storage);
extern void memory_buffer_free(memory_buffer_t *head);
extern void memory_buffer_print(memory_buffer_t *head, char *mode);
extern void memory_buffer_print_r(char *path, memory_buffer_t *head, char *mode);

#endif


