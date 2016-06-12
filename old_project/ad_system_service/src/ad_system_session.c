
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#include "lg_string.h"
#include "ad_system_session.h"

lg_rbtree_head ad_system_session_head = RB_ROOT;
pthread_mutex_t ad_system_session_lock = PTHREAD_MUTEX_INITIALIZER;

ad_system_session_t *ad_system_session_add()
{

    lg_rbtree_node *parent = NULL;
    lg_rbtree_node **p = &ad_system_session_head.rb_node;
    ad_system_session_t *node = NULL;

    char session_id[40] = {0};
    lg_string_uuid(session_id);

    pthread_mutex_lock(&ad_system_session_lock);

    while (*p) {

	parent = *p;
	node = (ad_system_session_t *)parent;
	int retval = strcmp(session_id, node->session_id);

	if (retval < 0) {

	    p = &(*p)->rb_left;

	} else if (retval > 0) {

	    p = &(*p)->rb_right;

	} else {

	    pthread_mutex_unlock(&ad_system_session_lock);
	    return NULL;
	}
    }

    if (!(node = malloc(sizeof(ad_system_session_t)))) {

	pthread_mutex_unlock(&ad_system_session_lock);
	return NULL;
    }

    memset(node, 0, sizeof(ad_system_session_t));
    strcpy(node->session_id, session_id);
    node->time = time(NULL);

    lg_rbtree_link_node(&node->tree, parent, p);
    lg_rbtree_insert_color(&node->tree, &ad_system_session_head);

    pthread_mutex_unlock(&ad_system_session_lock);
    return node;
}

ad_system_session_t *ad_system_session_find(char *session_id)
{

    lg_rbtree_node *p = ad_system_session_head.rb_node;
    ad_system_session_t *node = NULL;

    pthread_mutex_lock(&ad_system_session_lock);

    while (p) {

	node = (ad_system_session_t *)p;
	int retval = strcmp(session_id, node->session_id);

	if (retval < 0) {

	    p = p->rb_left;

	} else if (retval > 0) {

	    p = p->rb_right;

	} else {

	    if (time(NULL) - node->time > 3600) {

		lg_rbtree_delete(&ad_system_session_head, &node->tree);
		pthread_mutex_unlock(&ad_system_session_lock);

		free(node);
		return NULL;
	    }

	    pthread_mutex_unlock(&ad_system_session_lock);
	    node->time = time(NULL);
	    return node;
	}
    }

    pthread_mutex_unlock(&ad_system_session_lock);
    return NULL;
}

void _ad_system_session_print(lg_rbtree_node *p)
{

    ad_system_session_t *node = (ad_system_session_t *)p;
    fprintf(stdout, "[session list][%s][%s]\n", node->session_id, node->account_id);

    if (p->rb_left) _ad_system_session_print(p->rb_left);
    if (p->rb_right)_ad_system_session_print(p->rb_right);
}

void ad_system_session_print()
{

    if (!ad_system_session_head.rb_node)
	return;

    pthread_mutex_lock(&ad_system_session_lock);
    _ad_system_session_print(ad_system_session_head.rb_node);
    pthread_mutex_unlock(&ad_system_session_lock);
}

char *ad_system_get_session(char *cookies, char *session)
{

    char *str = NULL;
    char *key = NULL;
    char *save = NULL;
    char *buffer = cookies;
    while((str =  strtok_r(buffer, ";", &save))) {

	if ((key = strstr(str, "key="))) {

	    strcpy(session, key + 4);
	    return session;
	}

	buffer = NULL;
    }

    return NULL;
}

ad_system_session_t *ad_system_session_parse(char *cookies)
{

    char session[1024] = "null";
    if (!(ad_system_get_session(cookies, session)))
	goto NEW_SESSION;

    ad_system_session_t *node = NULL;
    if ((node = ad_system_session_find(session)))
	return node;

NEW_SESSION:
    if ((node = ad_system_session_add()))
	return node;

    return NULL;
}


