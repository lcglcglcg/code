
#include "scws.h"
#include "lg_election.h"
#include <stdlib.h>

void lg_election_init(lg_election_head_t *head)
{
    head->total = 0;
    head->head_tree = RB_ROOT;
    lg_list_init(&head->head_list);
}

int lg_election_add(lg_election_head_t *head, char *buffer, int size)
{

    int row = head->total++;
    lg_rbtree_node *parent = NULL;
    lg_rbtree_node **node = &head->head_tree.rb_node;
    lg_election_t *temp = NULL;

    while (*node) {

	parent = *node;
	temp = rb_entry(parent, lg_election_t, tree);

	if (row < temp->row)
	    node = &(*node)->rb_left;
	else if (row > temp->row)
	    node = &(*node)->rb_right;
	else
	    return -1;
    }

    if (!(temp = malloc(sizeof(lg_election_t))))
	return -1;

    temp->row = row;
    temp->size = size;
    temp->buffer = malloc(size + 1);
    if (!temp->buffer) return -1;
    memcpy(temp->buffer, buffer, size);
    temp->buffer[size] = 0;

    lg_rbtree_link_node(&temp->tree, parent, node);
    lg_rbtree_insert_color(&temp->tree, &head->head_tree);
    lg_list_add(&head->head_list, &temp->next);
    return 0;
}

lg_election_t *lg_election_find(lg_election_head_t *head, size_t row)
{

    lg_rbtree_node *node = head->head_tree.rb_node;
    lg_election_t *temp = NULL;

    while (node) {

	temp = rb_entry(node, lg_election_t, tree);

	if (row < temp->row)
	    node = node->rb_left;
	else if (row > temp->row)
	    node = node->rb_right;
	else
	    return temp;
    }

    return NULL;
}

int lg_election_load(lg_election_head_t *head, char *buffer, int size)
{

    scws_t s;
    scws_res_t res, cur;
    if (!(s = scws_new()))
	return -1;

    scws_set_charset(s, "utf-8");
    scws_set_dict(s, "dict.utf8.xdb", SCWS_XDICT_XDB);
    scws_send_text(s, buffer, size);

    while ((res = cur = scws_get_result(s))) {

	while (cur != NULL) {

	    if (cur->len >= 6)
		lg_election_add(head, buffer + cur->off, cur->len);

	    cur = cur->next;
	}

	scws_free_result(res);
    }

    scws_free(s);
    return 0;
}

void lg_election_print(lg_election_head_t *head)
{

    lg_list_t *p = NULL;
    lg_election_t *node = NULL;
    lg_list_for_each(p, &head->head_list) {

	node = lg_list_entry(p, lg_election_t, next);
	fprintf(stdout, "[%d][%s]\n", node->row, node->buffer);
    }

    fprintf(stdout, "total=%d\n", head->total);
}

void lg_election_free(lg_election_head_t *head)
{
    lg_election_t *node = NULL;
    lg_list_t *p = head->head_list.next;
    while(p != &head->head_list) {

	node = (lg_election_t *)p;
	p = p->next;

	free(node->buffer);
	free(node);
    }

    head->head_tree = RB_ROOT;
    lg_list_init(&head->head_list);
}

lg_election_head_t *lg_election_load_file(const char *path)
{

    lg_election_head_t *head = malloc(sizeof(lg_election_head_t));
    if (!head) return NULL;

    head->total = 0;
    head->head_tree = RB_ROOT;
    lg_list_init(&head->head_list);

    FILE *fp = fopen("dict.utf8.keyword", "r");
    if (!fp) return NULL;

    int size = 0;
    char buffer[4096];
    while((size = fread(buffer, 1, 4096, fp)))
	lg_election_load(head, buffer, size);

    fclose(fp);
    return head;
}


