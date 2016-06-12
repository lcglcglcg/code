
#ifndef __LG_DB_H__
#define __LG_DB_H__

#include "lg_list.h"
#include "lg_rbtree.h"
#include <stdlib.h>

typedef void(lg_db_free_fun)(lg_list_t *data_list, void *data);

typedef struct {

    lg_rbtree_node	tree;
    lg_list_t 		next;

    lg_rbtree_head 	head_tree;	//子节点
    lg_list_t 		head_list;	//子节点

    lg_list_t		m_data_list;	// 数据 list
    void 		*m_data;	// 数据

} lg_db_node_t;

typedef struct {

    lg_rbtree_head 	head_tree;
    lg_list_t 		head_list;

} lg_db_t;


// db create 
extern lg_db_t *lg_db_create();
// db free
extern void lg_db_free(lg_db_t *head, lg_db_free_fun *fun);

// add number
extern lg_db_node_t *lg_db_number_node_add(lg_db_t *head, size_t index);
// add number child
extern lg_db_node_t *lg_db_number_node_child_add(lg_db_node_t *head, size_t index);
// find number
extern lg_db_node_t *lg_db_number_find(lg_db_t *head, size_t index);
// find number child
extern lg_db_node_t *lg_db_number_child_find(lg_db_node_t *head, size_t index);

// add string
extern lg_db_node_t *lg_db_string_node_add(lg_db_t *head, char *key);
// add string child
extern lg_db_node_t *lg_db_string_node_child_add(lg_db_node_t *head, char *key);
// find string
extern lg_db_node_t *lg_db_string_find(lg_db_t *head, char *key);
// find string child
extern lg_db_node_t *lg_db_string_child_find(lg_db_node_t *head, char *key);

#endif


