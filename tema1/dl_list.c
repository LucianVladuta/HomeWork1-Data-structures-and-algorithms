#include "dl_list.h"
#include <stdlib.h>

struct list * list_create(cb_allocator allocator, cb_deallocator deallocator, cb_data_copy data_copy)
{
	struct list *tmp = (struct list *)calloc(1, sizeof(struct list));


	tmp->alloc = allocator;
	tmp->dealloc = deallocator;
	tmp->data_copy = data_copy;
	return tmp;
}

void list_delete(struct list *lst)
{
	struct node *current;

	if (!lst)
		return;
	
	current = lst->tail;
	while (current)
	{
		struct node *tmp = current;
		lst->dealloc(tmp->data);
		current = current->prev;
		free(tmp);
	}

	free(lst);
}

void * list_pop_back(struct list *lst)
{
	struct node *item = NULL;
	void *data = NULL;

	if (lst->tail)
	{
		item = lst->tail;
		if (lst->tail == lst->head)
		{
			lst->tail = lst->head = NULL;
		}
		else
		{
			lst->tail = item->prev;
			lst->tail->next = NULL;
		}
	}

	if (item)
	{
		data = item->data;
		free(item);
	}
	
	lst->size--;	
	return data;
}

void list_push_back(struct list *lst, void *data)
{
	struct node *new_item = (struct node *)calloc(1, sizeof(struct node));

	new_item->data = lst->alloc();
	lst->data_copy(data, new_item->data);

	if (lst->tail)
	{
		lst->tail->next = new_item;
		new_item->prev = lst->tail;
		lst->tail = new_item;
	}
	else
	{
		lst->tail = lst->head = new_item;
	}

	lst->size++;
}

void * list_pop_front(struct list *lst)
{
	struct node *item = NULL;
	void *data = NULL;
	if (lst->head)
	{
		item = lst->head;
		if (lst->tail == lst->head)
		{
			lst->tail = lst->head = NULL;
		}
		else
		{
			lst->head = item->next;
			lst->head->prev = NULL;
		}
	}

	if (item)
	{
		data = item->data;
		free(item);
	}

	lst->size--;

	return data;
}

void list_push_front(struct list *lst, void *data)
{
	struct node *new_item = (struct node *)calloc(1, sizeof(struct node));

	new_item->data = lst->alloc();
	lst->data_copy(data, new_item->data);

	if (lst->head)
	{
		lst->head->prev = new_item;
		new_item->next = lst->head;
		lst->head = new_item;
	}
	else
	{
		lst->tail = lst->head = new_item;
	}

	lst->size++;
}

void * list_peek_front(struct list *lst)
{
	void *data = NULL;
	if (lst && lst->head)
		data = lst->head->data;
	return data;
}

void * list_peek_back(struct list *lst)
{
	void *data = NULL;
	if (lst && lst->tail)
		data = lst->tail->data;
	return data;
}


void * list_find(struct list *lst, cb_find test_fn, void *look_for)
{
	struct node *iter;
	
	if (!lst)
		return NULL;

	for (iter = lst->head; iter; iter = iter->next)
	{
		if (test_fn(iter->data, look_for))
			return iter->data;
	}

	return NULL;
}

void list_iterate_data(struct list *lst, cb_iter iter_fn)
{
	if (!lst || !iter_fn)
		return;

	struct node *iter;
	for (iter = lst->head; iter; iter = iter->next)
		iter_fn(iter->data);
}

void list_iterate_nodes(struct list *lst, cb_iter iter_fn)
{
	struct node *iter;
	for (iter = lst->head; iter; iter = iter->next)
		iter_fn(iter);
}

void list_remove_node(struct list *lst, struct node *item)
{
	if (!lst || !item)
		return;
	
	if (!lst->head && !lst->tail)
		return;

	if (lst->head == item && lst->tail == item)
	{
		lst->dealloc(item->data);
		free(item);
		lst->head = lst->tail = NULL;
		lst->size--;
		return;
	}

	if (lst->head == item)
	{
		lst->head = lst->head->next;
		lst->head->prev = NULL;
		lst->dealloc(item->data);
		free(item);
		lst->size--;
		return;
	}

	if (lst->tail == item)
	{
		lst->tail = lst->tail->prev;
		lst->tail->next = NULL;
		lst->dealloc(item->data);
		free(item);
		lst->size--;
		return;
	}

	struct node *iter;
	for (iter = lst->head; iter; iter = iter->next)
		if (iter == item)
		{
			iter->prev->next = iter->next;
			iter->next->prev = iter->prev;
			lst->dealloc(iter->data);
			free(iter);
			lst->size--;
			break;
		}
}

struct node * list_get_first(struct list *lst)
{
	if (!lst)
		return NULL;
	return lst->head;
}

struct node * list_get_next(struct node *item)
{
	if (!item)
		return NULL;

	return item->next;
}

struct node * list_get_last(struct list *lst)
{
	if (!lst)
		return NULL;
	return lst->tail;
}

struct node * list_get_prev(struct node *item)
{
	if (!item)
		return NULL;

	return item->prev;
}
