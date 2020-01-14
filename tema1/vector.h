#pragma once

#include "cb_defs.h"

struct vector
{
	void *data;
	int element_size;
	int size;

	cb_allocator alloc;
	cb_deallocator dealloc;
	cb_data_copy data_copy;
};

struct vector * vector_create(int element_size, cb_deallocator deallocator, cb_data_copy data_copy);
void vector_delete(struct vector *v);
//void vector_ensure_capacity(struct vector *v, int new_size);
void * vector_item(struct vector *v, int idx);
void * vector_end(struct vector *v);
void vector_push_back(struct vector *v, void *data);
//void vector_erase(struct vector *v, int idx);
void vector_pop_back(struct vector *v);