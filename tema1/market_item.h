#pragma once
#include <stdlib.h>
#include <string.h>

struct market_item
{
	char *nume;
	char *categorie;
	int pret;
	int cantitate;
	int stoc;
};

static inline void * market_allocator()
{
	return calloc(1, sizeof(struct market_item));
}

static inline void market_deallocator(void *data)
{
	struct market_item *item = (struct market_item*)data;
	
	if (item->nume)
		free(item->nume);
	if (item->categorie)
		free(item->categorie);
}

static inline void market_copy(void *src, void *dst)
{
	struct market_item *mi_src = (struct market_item *)src;
	struct market_item *mi_dst = (struct market_item *)dst;
	mi_dst->nume = (char *)strdup(mi_src->nume);
	mi_dst->categorie = strdup(mi_src->categorie);
	mi_dst->pret = mi_src->pret;
	mi_dst->cantitate = mi_src->cantitate;
	mi_dst->stoc = mi_src->stoc;
}