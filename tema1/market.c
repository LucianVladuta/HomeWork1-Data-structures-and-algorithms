#include <stdlib.h>

#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "cb_defs.h"
#include "dl_list.h"
#include "stack.h"
#include "vector.h"

#include "market_item.h"
#include "buy_item.h"
#include "element.h"

FILE *output;

#define STATIC_STRING_SIZE 1024

enum {
	CERINTA1 = 0x01,
	CERINTA2 = 0x02,
	CERINTA3 = 0x04,
	CERINTA4 = 0x08,
	CERINTA5 = 0x10,
	CERINTA6 = 0x20
};

unsigned cerinte;
int buget;


void market_item_print(void *data)
{
	struct market_item *item = (struct market_item *)data;
	fprintf(output,"%s %s %d %d %d\n", item->nume, item->categorie, item->pret,
		item->cantitate, item->stoc);
}

int market_gaseste_categorie(void *data, void *look_for)
{
	struct market_item *item = (struct market_item *)data;
	char *lf = (char *)look_for;
	
	if (!strcmp(item->categorie, lf))
		return 1;

	return 0;
}


struct list * construieste_lista_produse_supermarket(const char *file_name)
{
	struct list *tmp_list;
	FILE *file = fopen(file_name, "rt");

	if (!file)
	{
		fprintf(output, "Nu se poate deschide fisierul %s\n", file_name);
		return NULL;
	}

	tmp_list = list_create(market_allocator, market_deallocator, market_copy);

	int items;
	fscanf(file, "%d", &items);

	char nume[STATIC_STRING_SIZE], categorie[STATIC_STRING_SIZE];
	int pret;
	int cantitate;
	int stoc;
	struct market_item mi;
	int i;
	for (i = 0; i < items; i++)
	{
		fscanf(file, "%s %s %d %d %d", nume, categorie, &pret, &cantitate, &stoc);
		mi.nume = nume;
		mi.categorie = categorie;
		mi.pret = pret;
		mi.cantitate = cantitate;
		mi.stoc = stoc;
		list_push_back(tmp_list, &mi);
	}

	fclose(file);
	return tmp_list;
}

void buy_item_print(void *data)
{
	struct buy_item *item = (struct buy_item *)data;
	fprintf(output, "%s %s %d\n", item->nume, item->categorie, item->cantitate);
}


struct list * construieste_lista_cumparaturi(const char *file_name)
{
	struct list *tmp_list;
	FILE *file = fopen(file_name, "rt");

	if (!file)
	{
		fprintf(output, "Nu se poate deschide fisierul %s\n", file_name);
		return NULL;
	}

	tmp_list = list_create(buy_item_allocator, buy_item_deallocator, buy_item_copy);

	int items;
	fscanf(file, "%d", &items);

	char nume[STATIC_STRING_SIZE], categorie[STATIC_STRING_SIZE];
	int cantitate;
	
	struct buy_item bi;
	int i;

	for (i = 0; i < items; i++)
	{
		fscanf(file, "%s %s %d", nume, categorie, &cantitate);
		bi.nume = nume;
		bi.categorie = categorie;
		bi.cantitate = cantitate;
		list_push_back(tmp_list, &bi);
	}

	fclose(file);
	return tmp_list;
}

void parse_rules(const char *file_name)
{
	FILE *file = fopen(file_name, "rt");

	if (!file)
	{
		fprintf(output, "Nu se poate deschide fisierul %s\n", file_name);
		return;
	}

	int c[6];

	fscanf(file, "%d %d %d %d %d %d", &c[0], &c[1], &c[2], &c[3], &c[4], &c[5]);
	fscanf(file, "%d", &buget);
	
	#define SET_BIT(X,N) ( (X) |=  (1 << (N) ) )
	
	cerinte = 0;
	int i;
	for(i = 0; i <6; i++)
		if (c[i])
			SET_BIT(cerinte, i);

	fclose(file);
}

int produs_fara_stoc(void *data, void *look_for)
{
	struct market_item *m_item = (struct market_item *)data;
	struct buy_item *b_item = (struct buy_item *)look_for;
	
	if (!b_item || !m_item)
		return 0;
	
	if (!strcmp(m_item->nume, b_item->nume) &&
		!strcmp(m_item->categorie, b_item->categorie) &&
		m_item->stoc == 0)
		return 1;
	return 0;
}


void elimina_produse_fara_stoc(struct list * lista_cumparaturi, struct list *lista_market)
{
	struct node *buy_node;

	buy_node = list_get_first(lista_cumparaturi);

	if (!buy_node)
		return;

	while (buy_node)
	{
		struct node *tmp = buy_node;

		buy_node = list_get_next(buy_node);

		if (list_find(lista_market, produs_fara_stoc, tmp->data))
			list_remove_node(lista_cumparaturi, tmp);
	}
}

struct list * cumpara_cu_buget(int budget, struct list *lista_cumparaturi, struct list *lista_market)
{
	struct node *buy_node;
	struct node *market_node;
	struct list *produse_cumparate;
	
	if (!lista_cumparaturi || !lista_market)
		return NULL;

	buy_node = list_get_first(lista_cumparaturi);

	if (!buy_node)
		return NULL;

	produse_cumparate = list_create(buy_item_allocator, buy_item_deallocator, buy_item_copy);

	while (buy_node)
	{
		struct buy_item *b_item = (struct buy_item *)buy_node->data;

		market_node = list_get_first(lista_market);

		while (market_node)
		{
			struct market_item *m_item = (struct market_item*)market_node->data;
			if (!strcmp(b_item->nume, m_item->nume) && !strcmp(b_item->categorie, m_item->categorie))
			{

				int cantitate;
				
				if (b_item->cantitate <= m_item->cantitate)
					cantitate = b_item->cantitate;
				else
					cantitate = m_item->cantitate;

				if (buget - cantitate * m_item->pret < 0)
					cantitate = buget / m_item->pret;

				if(cantitate)
				{
					struct buy_item *tmp;

					tmp = (struct buy_item *) buy_item_allocator();
					buy_item_copy(b_item, tmp);
					tmp->cantitate = cantitate;

					list_push_back(produse_cumparate, tmp);
					buy_item_deallocator(tmp);
					free(tmp);
					buget -= cantitate * m_item->pret;
				}
			}
			market_node = list_get_next(market_node);
		}

		buy_node = list_get_next(buy_node);
	}

	return produse_cumparate;
}


void * gaseste_produs_cu_pret_apropiat(int pret, char * nume, char *categorie, struct list *lista_market)
{

	struct market_item *f_item = NULL;
	struct node *m_node = NULL;

	int delta = INT_MAX;

	if(!lista_market)
		return NULL;

	m_node = list_get_first(lista_market);

	while (m_node)
	{
		struct market_item *m_item = (struct market_item *)m_node->data;
		if (!strcmp(categorie, m_item->categorie) && strcmp(nume, m_item->nume))
		{
			if (abs(pret - m_item->pret) <= delta)
			{
				delta = abs(pret - m_item->pret);
				f_item = m_item;
			}
		}
		m_node = list_get_next(m_node);
	}

	return f_item;
}

struct list * cumpara_fara_limite(struct list *lista_cumparaturi, struct list *lista_market)
{
	struct node *buy_node;
	struct node *market_node;
	struct list *produse_cumparate;

	if (!lista_cumparaturi || !lista_market)
		return NULL;

	buy_node = list_get_first(lista_cumparaturi);

	if (!buy_node)
		return NULL;

	produse_cumparate = list_create(buy_item_allocator, buy_item_deallocator, buy_item_copy);
	while (buy_node)
	{
		struct buy_item *b_item = (struct buy_item *)buy_node->data;
		int pret = 0;

		market_node = list_get_first(lista_market);

		while (market_node)
		{
			struct market_item *m_item = (struct market_item*)market_node->data;
			if (!strcmp(b_item->nume, m_item->nume) && !strcmp(b_item->categorie, m_item->categorie))
			{
				pret = m_item->pret;

				if (b_item->cantitate <= m_item->cantitate)
				{
					list_push_back(produse_cumparate, b_item);
					b_item->cantitate = 0;
				}
				else
				{
					int tmp = b_item->cantitate;

					b_item->cantitate = m_item->cantitate;
					list_push_back(produse_cumparate, b_item);
					b_item->cantitate = tmp - m_item->cantitate;
				}
			}
			market_node = list_get_next(market_node);
		}

		if (b_item->cantitate)
		{
			struct market_item *m_item;

			m_item = (struct market_item *)gaseste_produs_cu_pret_apropiat(pret, b_item->nume, b_item->categorie, lista_market);

			if (m_item)
			{
				if (b_item->cantitate <= m_item->cantitate)
				{
					struct buy_item *tmp;

					tmp = (struct buy_item *) buy_item_allocator();
					
					tmp->nume = strdup(m_item->nume);
					tmp->categorie = strdup(m_item->categorie);
					tmp->cantitate = b_item->cantitate;
					
					list_push_back(produse_cumparate, tmp);

					buy_item_deallocator(tmp);
					free(tmp);
					b_item->cantitate = 0;
				}
			}
		}

		buy_node = list_get_next(buy_node);
	}

	return produse_cumparate;
}

struct stack *creeaza_stiva_preturi(struct list *lista_market)
{
	struct stack *stiva_compara_preturi = stack_create(element_allocator, element_deallocator, element_copy);
	struct node *market_node;
	struct node *prev_node;

	market_node = list_get_first(lista_market);

	while (market_node)
	{
		prev_node = list_get_prev(market_node);

		struct market_item *mi = (struct market_item *)market_node->data;

		int count = 0;
		//int pret = mi->pret;

		while (prev_node)
		{
			struct market_item *mip = (struct market_item*)prev_node->data;

			if (mip->pret > mi->pret)
				count++;
			else
				break;
			prev_node = list_get_prev(prev_node);
		}

		struct element si;
		si.val = count;
		stack_push(stiva_compara_preturi, &si);
		market_node = list_get_next(market_node);
	}

	return stiva_compara_preturi;
}

struct vector * creeaza_vector_din_stiva(struct stack *n_stack)
{
	struct node *item;


	if (!n_stack)
		return NULL;

	struct vector *v = vector_create(sizeof(struct element), element_deallocator, element_copy);

	item = stack_get_base(n_stack);


	while (item)
	{
		struct element *si = (struct element *)item->data;
		vector_push_back(v, si);
		item = stack_get_next(item);
	}

	return v;
}

void afiseaza_vector_preturi_comparate(struct vector *v)
{
	int i;

	for(i = 0 ; i < v->size; i++)
	{
		struct element * el = (struct element *)vector_item(v, i);
		fprintf(output, "%d ", el->val);
	}
}

int main(int argc, char * argv [])
{

	struct list *lista_market = NULL;
	struct list *lista_cumparaturi = NULL;
	struct list *produse_cumparate = NULL;
	struct stack *stiva_compara_preturi = NULL;
	struct vector *vector_compara_preturi = NULL;


	if(argc < 5)
		return -1;

	const char *f_date1 = argv[1];
	const char *f_date2 = argv[2];
	const char *f_cerinte = argv[3];
	const char *f_rezultate = argv[4];

	output = fopen(f_rezultate,"wt");
	if (!output)
		return -1;

	lista_market = construieste_lista_produse_supermarket(f_date1);

	parse_rules(f_cerinte);
	
	if (cerinte & CERINTA1)
		lista_cumparaturi = construieste_lista_cumparaturi(f_date2);
	
	if(cerinte & CERINTA2)
		elimina_produse_fara_stoc(lista_cumparaturi, lista_market);
	
	if (cerinte & CERINTA3)
		produse_cumparate = cumpara_cu_buget(buget, lista_cumparaturi, lista_market);

	if (cerinte & CERINTA4)
	{
		if (produse_cumparate)
		{
			list_delete(produse_cumparate);
		}
		produse_cumparate = cumpara_fara_limite(lista_cumparaturi, lista_market);
	}

	if (cerinte & CERINTA5)
	{

	}

	if (cerinte & CERINTA6)
	{
		stiva_compara_preturi = creeaza_stiva_preturi(lista_market);
		vector_compara_preturi = creeaza_vector_din_stiva(stiva_compara_preturi);
	}

	if (cerinte & CERINTA1 || cerinte & CERINTA2)
	{
		list_iterate_data(lista_cumparaturi, buy_item_print);
		fprintf(output, "\n");
	}

	if (cerinte & CERINTA3 || cerinte & CERINTA4)
	{
		list_iterate_data(produse_cumparate, buy_item_print);
		fprintf(output, "\n");
	}

	if (cerinte & CERINTA5)
	{
		fprintf(output, "\n\n");
	}

	if (cerinte & CERINTA6)
	{
		afiseaza_vector_preturi_comparate(vector_compara_preturi);
		fprintf(output, "\n");
	}

	list_delete(produse_cumparate);
	list_delete(lista_market);
	list_delete(lista_cumparaturi);
	stack_delete(stiva_compara_preturi);
	vector_delete(vector_compara_preturi);
	
	fclose(output);

	return 0;
}

