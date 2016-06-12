
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "lg_rbtree.h"

typedef struct {

    lg_rbtree_node node;
    
    unsigned int ip_s;
    unsigned int ip_e;

}ip_restrict_t;

int ip_restrict_add(lg_rbtree_head *head, ip_restrict_t *ip_node)
{

    ip_restrict_t *parent = NULL;
    lg_rbtree_node **node = &head->rb_node;

    while (*node) {

	parent = (ip_restrict_t *)*node;

	if (ip_node->ip_s < parent->ip_s)
	    node = &(*node)->rb_left;
	else if (ip_node->ip_e > parent->ip_s)
	    node = &(*node)->rb_right;
	else
	    return -1;
    }

    lg_rbtree_link_node(&ip_node->node, &parent->node, node);
    lg_rbtree_insert_color(&ip_node->node, head);
    return 0;
}

ip_restrict_t *ip_restrict_find(lg_rbtree_head *head, int ip_addr)
{

    lg_rbtree_node *node = head->rb_node;

    while (node) {

	ip_restrict_t *ip_node = (ip_restrict_t *)node;

	if (ip_addr >= ip_node->ip_s && ip_addr <= ip_node->ip_e)
	    return ip_node;
	else if (ip_addr < ip_node->ip_s)
	    node = node->rb_left;
	else if (ip_addr > ip_node->ip_s)
	    node = node->rb_right;
	else
	    return NULL;
    }

    return NULL;
}

