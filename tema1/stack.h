#pragma once

#include <stdlib.h>
#include "dl_list.h"

struct stack
{
	struct list *dl_list;
};

static inline struct stack * stack_create(cb_allocator allocator, cb_deallocator deallocator, cb_data_copy data_copy)
{
	struct stack *new_stack = (struct stack *)calloc(1, sizeof(struct stack));
	new_stack->dl_list = list_create(allocator, deallocator, data_copy);

	return new_stack;
}

static inline void stack_delete(struct stack *n_stack)
{
	if (!n_stack)
		return;
	list_delete(n_stack->dl_list);
	free(n_stack);
}

static inline void stack_push(struct stack *n_stack, void *data)
{
	if (n_stack && n_stack->dl_list)
		list_push_front(n_stack->dl_list, data);
}

static inline void * stack_pop(struct stack *n_stack)
{
	void *data = NULL;
	
	if (n_stack && n_stack->dl_list)
		data = list_pop_front(n_stack->dl_list);

	return data;
}

static inline void * stack_peek(struct stack *n_stack)
{
	return list_peek_front(n_stack->dl_list);
}

static inline struct node * stack_get_top(struct stack *n_stack)
{
	struct node *item = NULL;
	if (n_stack && n_stack->dl_list)
		return list_get_first(n_stack->dl_list);
	return item;
}

static inline struct node * stack_get_base(struct stack *n_stack)
{
	struct node *item = NULL;
	if (n_stack && n_stack->dl_list)
		return list_get_last(n_stack->dl_list);
	return item;
}
static inline struct node * stack_get_prev(struct node *item)
{
	return list_get_next(item);
}

static inline struct node * stack_get_next(struct node *item)
{
	return list_get_prev(item);
}

static inline int stack_get_size(struct stack *n_stack)
{
	if (!n_stack)
		return -1;
	return list_get_size(n_stack->dl_list);
}
