
#include "lg_db.h"

// db create 
lg_db_t *lg_db_create()
{

    // 创建新的head
    lg_db_t *head = malloc(sizeof(lg_db_t));
    if (!head) return NULL;

    head->head_tree = RB_ROOT;
    lg_list_init(&head->head_list);
    return head;
}

// add number
lg_db_node_t *lg_db_number_node_add(lg_db_t *head, size_t index)
{

    lg_db_node_t *node = malloc(sizeof(lg_db_node_t));
    if (!node) return NULL;

    node->tree.key = NULL;
    node->tree.index = index;

    lg_tree_element_index_add(&head->head_tree, &node->tree);
    lg_list_add(&head->head_list, &node->next);

    node->head_tree = RB_ROOT;
    lg_list_init(&node->head_list);

    lg_list_init(&node->m_data_list);
    node->m_data = NULL;
    return node;
}

// add number child
lg_db_node_t *lg_db_number_node_child_add(lg_db_node_t *head, size_t index)
{
    lg_db_node_t *node = malloc(sizeof(lg_db_node_t));
    if (!node) return NULL;

    node->tree.key = NULL;
    node->tree.index = index;

    lg_tree_element_index_add(&head->head_tree, &node->tree);
    lg_list_add(&head->head_list, &node->next);

    node->head_tree = RB_ROOT;
    lg_list_init(&node->head_list);

    lg_list_init(&node->m_data_list);
    node->m_data = NULL;
    return node;
}

// find number
lg_db_node_t *lg_db_number_find(lg_db_t *head, size_t index)
{
    if (!head) return NULL;

    lg_rbtree_node *node = lg_tree_element_index_find(&head->head_tree, index);
    if (!node) return NULL;

    return rb_entry(node, lg_db_node_t, tree);
}

// find number child
lg_db_node_t *lg_db_number_child_find(lg_db_node_t *head, size_t index)
{
    if (!head) return NULL;

    lg_rbtree_node *node = lg_tree_element_index_find(&head->head_tree, index);
    if (!node) return NULL;

    return rb_entry(node, lg_db_node_t, tree);
}

// add string
lg_db_node_t *lg_db_string_node_add(lg_db_t *head, char *key)
{

    lg_db_node_t *node = malloc(sizeof(lg_db_node_t));
    if (!node) return NULL;

    node->tree.index = 0;
    node->tree.key = malloc(strlen(key) + 1);
    if (!node->tree.key) {

	free(node);
	return NULL;
    }

    strcpy(node->tree.key, key);
    lg_tree_element_key_add(&head->head_tree, &node->tree);
    lg_list_add(&head->head_list, &node->next);

    node->head_tree = RB_ROOT;
    lg_list_init(&node->head_list);

    lg_list_init(&node->m_data_list);
    node->m_data = NULL;
    return node;
}

// add string child
lg_db_node_t *lg_db_string_node_child_add(lg_db_node_t *head, char *key)
{
    if (!key) return NULL;

    lg_db_node_t *node = malloc(sizeof(lg_db_node_t));
    if (!node) return NULL;

    node->tree.index = 0;
    node->tree.key = malloc(strlen(key) + 1);
    if (!node->tree.key) {

	free(node);
	return NULL;
    }

    strcpy(node->tree.key, key);
    lg_tree_element_key_add(&head->head_tree, &node->tree);
    lg_list_add(&head->head_list, &node->next);

    node->head_tree = RB_ROOT;
    lg_list_init(&node->head_list);

    lg_list_init(&node->m_data_list);
    node->m_data = NULL;
    return node;
}

// find string
lg_db_node_t *lg_db_string_find(lg_db_t *head, char *key)
{

    if (!head) return NULL;

    lg_rbtree_node *node = lg_tree_element_key_find(&head->head_tree, key);
    if (!node) return NULL;

    return rb_entry(node, lg_db_node_t, tree);
}

// find string child
lg_db_node_t *lg_db_string_child_find(lg_db_node_t *head, char *key)
{
    if (!head) return NULL;

    lg_rbtree_node *node = lg_tree_element_key_find(&head->head_tree, key);
    if (!node) return NULL;

    return rb_entry(node, lg_db_node_t, tree);
}

void lg_db_node_free(lg_db_node_t *head, lg_db_free_fun *fun) 
{

    lg_list_t *list_head = &head->head_list;
    lg_list_t *p = list_head->next;
    while(p != list_head) {

	lg_db_node_t *node = lg_list_entry(p, lg_db_node_t, next);
	p = p->next;

	// fprintf(stdout, "2==>%zd\n", node->tree.index);
	if (fun)fun(&node->m_data_list, node->m_data);
	lg_db_node_free(node, fun);
	free(node);
    }
}

void lg_db_free(lg_db_t *head, lg_db_free_fun *fun)
{

    lg_list_t *list_head = &head->head_list;
    lg_list_t *p = list_head->next;
    while(p != list_head) {

	lg_db_node_t *node = lg_list_entry(p, lg_db_node_t, next);
	p = p->next;

	// fprintf(stdout, "1==>%zd\n", node->tree.index);
	if (fun)fun(&node->m_data_list, node->m_data);
	lg_db_node_free(node, fun);
	free(node);
    }
}

#if 0
int main()
{

    lg_db_t *head = lg_db_create();

    lg_db_node_t *node = NULL;
    node = lg_db_number_node_add(head, 1);
    lg_db_number_node_child_add(node, 2);
    lg_db_number_node_child_add(node, 3);
    node = lg_db_number_node_child_add(node, 4);
    node = lg_db_number_node_child_add(node, 5);
    node = lg_db_number_node_child_add(node, 6);

    node = lg_db_number_node_add(head, 7);
    lg_db_number_node_child_add(node, 8);
    lg_db_number_node_child_add(node, 9);

    lg_db_free(head);
    return 0;
}
#endif




