
#ifndef __LG_ELECTION_H__
#define __LG_ELECTION_H__

#include "lg_list.h"
#include "lg_rbtree.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {

	lg_list_t next;
	lg_rbtree_node tree;

	int row;
	char *buffer;
	int size;

    } lg_election_t; 

    typedef struct {

	int total;
	lg_list_t head_list;
	lg_rbtree_head head_tree;

    } lg_election_head_t; 

    extern void lg_election_init(lg_election_head_t *head);
    extern int lg_election_load(lg_election_head_t *head, char *buffer, int size);
    extern lg_election_t *lg_election_find(lg_election_head_t *head, size_t row);
    extern void lg_election_free(lg_election_head_t *head);

    extern lg_election_head_t *lg_election_load_file(const char *path);

#ifdef __cplusplus
}
#endif

#endif


