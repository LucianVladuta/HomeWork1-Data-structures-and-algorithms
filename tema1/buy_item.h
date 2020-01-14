#pragma once

#include <stdlib.h>
#include <string.h>

struct buy_item
{
	char *nume;
	char *categorie;
	int cantitate;
};

static inline void * buy_item_allocator()
{
	return calloc(1, sizeof(struct buy_item));
}

static inline void buy_item_deallocator(void *data)
{
	struct buy_item *item = (struct buy_item*)data;

	if (item->nume)
		free(item->nume);
	if (item->categorie)
		free(item->categorie);
}

static inline void buy_item_copy(void *src, void *dst)
{
	struct buy_item *mi_src = (struct buy_item *)src;
	struct buy_item *mi_dst = (struct buy_item *)dst;
	mi_dst->nume = strdup(mi_src->nume);
	mi_dst->categorie = strdup(mi_src->categorie);
	mi_dst->cantitate = mi_src->cantitate;
}