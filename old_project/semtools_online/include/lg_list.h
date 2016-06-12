#ifndef	__LG_LIST_H__
#define	__LG_LIST_H__

typedef struct lg_list_head lg_list_t;

struct lg_list_head {
    struct lg_list_head *next, *prev;
};

static inline void INIT_LIST_HEAD(struct lg_list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void __lg_list_add(struct lg_list_head *new,
	struct lg_list_head *prev,
	struct lg_list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void lg_list_add(struct lg_list_head *new, struct lg_list_head *head)
{
    __lg_list_add(new, head, head->next);
}


static inline void lg_list_add_tail(struct lg_list_head *new, struct lg_list_head *head)
{
    __lg_list_add(new, head->prev, head);
}

static inline void __lg_list_del(struct lg_list_head * prev, struct lg_list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void __lg_list_del_entry(struct lg_list_head *entry)
{
    __lg_list_del(entry->prev, entry->next);
}

static inline void lg_list_del(struct lg_list_head *entry)
{
    __lg_list_del(entry->prev, entry->next);
    entry->next = (void *)0;
    entry->prev = (void *)0;
}

static inline void lg_list_replace(struct lg_list_head *old,
	struct lg_list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void lg_list_replace_init(struct lg_list_head *old,
	struct lg_list_head *new)
{
    lg_list_replace(old, new);
    INIT_LIST_HEAD(old);
}

static inline void lg_list_del_init(struct lg_list_head *entry)
{
    __lg_list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

static inline void lg_list_move(struct lg_list_head *list, struct lg_list_head *head)
{
    __lg_list_del_entry(list);
    lg_list_add(list, head);
}

static inline void lg_list_move_tail(struct lg_list_head *list,
	struct lg_list_head *head)
{
    __lg_list_del_entry(list);
    lg_list_add_tail(list, head);
}

static inline int lg_list_is_last(const struct lg_list_head *list,
	const struct lg_list_head *head)
{
    return list->next == head;
}

static inline int lg_list_empty(const struct lg_list_head *head)
{
    return head->next == head;
}

static inline int lg_list_empty_careful(const struct lg_list_head *head)
{
    struct lg_list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

static inline void lg_list_rotate_left(struct lg_list_head *head)
{
    struct lg_list_head *first;

    if (!lg_list_empty(head)) {
	first = head->next;
	lg_list_move_tail(first, head);
    }
}

static inline int lg_list_is_singular(const struct lg_list_head *head)
{
    return !lg_list_empty(head) && (head->next == head->prev);
}

static inline void __lg_list_cut_position(struct lg_list_head *list,
	struct lg_list_head *head, struct lg_list_head *entry)
{
    struct lg_list_head *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}

static inline void lg_list_cut_position(struct lg_list_head *list,
	struct lg_list_head *head, struct lg_list_head *entry)
{
    if (lg_list_empty(head))
	return;
    if (lg_list_is_singular(head) &&
	    (head->next != entry && head != entry))
	return;
    if (entry == head)
	INIT_LIST_HEAD(list);
    else
	__lg_list_cut_position(list, head, entry);
}

static inline void __lg_list_splice(const struct lg_list_head *list,
	struct lg_list_head *prev,
	struct lg_list_head *next)
{
    struct lg_list_head *first = list->next;
    struct lg_list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

static inline void lg_list_splice(const struct lg_list_head *list,
	struct lg_list_head *head)
{
    if (!lg_list_empty(list))
	__lg_list_splice(list, head, head->next);
}

static inline void lg_list_splice_tail(struct lg_list_head *list,
	struct lg_list_head *head)
{
    if (!lg_list_empty(list))
	__lg_list_splice(list, head->prev, head);
}

static inline void lg_list_splice_init(struct lg_list_head *list,
	struct lg_list_head *head)
{
    if (!lg_list_empty(list)) {
	__lg_list_splice(list, head, head->next);
	INIT_LIST_HEAD(list);
    }
}

static inline void lg_list_splice_tail_init(struct lg_list_head *list,
	struct lg_list_head *head)
{
    if (!lg_list_empty(list)) {
	__lg_list_splice(list, head->prev, head);
	INIT_LIST_HEAD(list);
    }
}

#define lg_list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define lg_list_first_entry(ptr, type, member) \
    lg_list_entry((ptr)->next, type, member)

#define lg_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define __lg_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define lg_list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define lg_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
	    pos = n, n = pos->next)

#define lg_list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
	    pos != (head); \
	    pos = n, n = pos->prev)

#define lg_list_for_each_entry(pos, head, member)				\
    for (pos = lg_list_entry((head)->next, typeof(*pos), member);	\
	    &pos->member != (head); 	\
	    pos = lg_list_entry(pos->member.next, typeof(*pos), member))

#define lg_list_for_each_entry_reverse(pos, head, member)			\
    for (pos = lg_list_entry((head)->prev, typeof(*pos), member);	\
	    &pos->member != (head); 	\
	    pos = lg_list_entry(pos->member.prev, typeof(*pos), member))

#define lg_list_prepare_entry(pos, head, member) \
    ((pos) ? : lg_list_entry(head, typeof(*pos), member))

#define lg_list_for_each_entry_continue(pos, head, member) 		\
    for (pos = lg_list_entry(pos->member.next, typeof(*pos), member);	\
	    &pos->member != (head);	\
	    pos = lg_list_entry(pos->member.next, typeof(*pos), member))

#define lg_list_for_each_entry_continue_reverse(pos, head, member)		\
    for (pos = lg_list_entry(pos->member.prev, typeof(*pos), member);	\
	    &pos->member != (head);	\
	    pos = lg_list_entry(pos->member.prev, typeof(*pos), member))

#define lg_list_for_each_entry_from(pos, head, member) 			\
    for (; &pos->member != (head);	\
	    pos = lg_list_entry(pos->member.next, typeof(*pos), member))

#define lg_list_for_each_entry_safe(pos, n, head, member)			\
    for (pos = lg_list_entry((head)->next, typeof(*pos), member),	\
	    n = lg_list_entry(pos->member.next, typeof(*pos), member);	\
	    &pos->member != (head); 					\
	    pos = n, n = lg_list_entry(n->member.next, typeof(*n), member))

#define lg_list_for_each_entry_safe_continue(pos, n, head, member) 		\
    for (pos = lg_list_entry(pos->member.next, typeof(*pos), member), 		\
	    n = lg_list_entry(pos->member.next, typeof(*pos), member);		\
	    &pos->member != (head);						\
	    pos = n, n = lg_list_entry(n->member.next, typeof(*n), member))

#define lg_list_for_each_entry_safe_from(pos, n, head, member) 			\
    for (n = lg_list_entry(pos->member.next, typeof(*pos), member);		\
	    &pos->member != (head);						\
	    pos = n, n = lg_list_entry(n->member.next, typeof(*n), member))

#define lg_list_for_each_entry_safe_reverse(pos, n, head, member)		\
    for (pos = lg_list_entry((head)->prev, typeof(*pos), member),	\
	    n = lg_list_entry(pos->member.prev, typeof(*pos), member);	\
	    &pos->member != (head); 					\
	    pos = n, n = lg_list_entry(n->member.prev, typeof(*n), member))

#define lg_list_safe_reset_next(pos, n, member)				\
    n = lg_list_entry(pos->member.next, typeof(*pos), member)

#define __offsetof(type,member) (size_t)(&((type *)0)->member)
#define rb_entry(ptr, type, member) ({const typeof(((type *)0)->member) * __mptr = (ptr);(type *)((char *)__mptr - __offsetof(type, member));})

#endif

