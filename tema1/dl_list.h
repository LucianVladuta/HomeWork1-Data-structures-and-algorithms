#pragma once

#include "cb_defs.h"

struct node
{
	struct node *prev, *next;
	void *data;
};

struct list
{
	struct node *head, *tail;
	int size;

	cb_allocator alloc;
	cb_deallocator dealloc;
	cb_data_copy data_copy;
};

struct list * list_create(cb_allocator allocator, cb_deallocator deallocator, cb_data_copy data_copy);
void list_delete(struct list *lst);

static inline int list_get_size(struct list *lst)
{
	if(!lst)
		return lst->size;
	return 0;
}

void * list_pop_back(struct list *lst);
void list_push_back(struct list *lst, void *data);

void * list_pop_front(struct list *lst);
void list_push_front(struct list *lst, void *data);

void * list_peek_front(struct list *lst);
void * list_peek_back(struct list *lst);

void * list_find(struct list *lst, cb_find test_fn, void *look_for);
void list_iterate_data(struct list *lst, cb_iter iter_fn);
void list_iterate_nodes(struct list *lst, cb_iter iter_fn);
void list_remove_node(struct list *lst, struct node *item);

struct node * list_get_first(struct list *lst);
struct node * list_get_next(struct node *item);
struct node * list_get_last(struct list *lst);
struct node * list_get_prev(struct node *item);