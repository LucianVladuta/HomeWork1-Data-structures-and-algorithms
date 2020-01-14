#include <stdlib.h>
#include <string.h>
#include "vector.h"

struct vector * vector_create(int element_size, cb_deallocator deallocator, cb_data_copy data_copy)
{
	struct vector *new_vector = (struct vector*)calloc(1,sizeof(struct vector));

	new_vector->dealloc = deallocator;
	new_vector->data_copy = data_copy;
	new_vector->element_size = element_size;

	return new_vector;
}

void vector_delete(struct vector *v)
{
	if (!v)
		return;

	if(!v->data)
		return;
	
	int i;
	for (i = 0 ; i < v->size; i++)
	{
		if(v->dealloc)
			v->dealloc(vector_item(v,i));
	}

	free(v->data);
	free(v);
}

void vector_ensure_capacity(struct vector *v, int new_size)
{
	void *ptr;
	
	if (new_size <= v->size)
		return;

	ptr = malloc(v->element_size * new_size);
	
    if (v->size)
		memcpy(ptr, v->data, v->element_size * v->size);
	
    if (v->data)
		free(v->data);
	
    v->data = ptr;
	v->size = new_size;
}

void * vector_item(struct vector *v, int idx)
{
	return (void*)(((char*)v->data) + v->element_size*idx);
}

void * vector_end(struct vector *v)
{
	if (!v->size)
		return NULL;

	return vector_item(v, v->size - 1);
}

void vector_erase(struct vector *v, int idx)
{
	
	if (idx >= v->size || !--v->size)
		return;

	v->dealloc(vector_item(v, idx));
	memmove(vector_item(v, idx),
			vector_item(v, idx+1),
			v->element_size*(v->size-idx));
}

void vector_push_back(struct vector *v, void *data)
{
	if (!v && !data)
		return;

	vector_ensure_capacity(v, v->size + 1);
	v->data_copy(data, vector_end(v));
}

 void vector_pop_back(struct vector *v)
{

	if (v->size)
		vector_erase(v, v->size-1);
}