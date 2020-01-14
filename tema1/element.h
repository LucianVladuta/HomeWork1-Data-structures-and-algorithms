#pragma once
#include <stdlib.h>

struct element
{
	int val;
};

static inline void * element_allocator()
{
	return calloc(1, sizeof(struct element));
}

static inline void element_copy(void *src, void *dst)
{
	struct element *si_src = (struct element *)src;
	struct element *si_dst = (struct element *)dst;
	si_dst->val = si_src->val;
}

static inline void element_deallocator(void *data)
{
	(void)data;
}