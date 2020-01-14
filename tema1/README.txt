
Pentru rezolvare cerintelor 1 2 3 4 6 au fost implementate urmatoarele structuri de date:
- lista dublu inlantuita generica : dl_list (dl_list.h dl_list.c)
- stiva generica: stack (stack.h) foloseste intern lista dublu inlantuita
- vector generic: vector (vector.h vector.c)

(termenul generic in acest context inseamna orice tip de date)

Pentru ca implementarea acestor structuri sa fie generica este necesara specificarea la crearea structurii de date a 3 functii:
	alocare - aloca o zona de memorie avand dimensiunea tipului de date folosit
	dealocare - in cazul in care tipul de data folosit contine alte tipuri de date alocate dinamic permite dealocarea 		    acestora
	copiere - metoda de copiere a obiectelor avand tipul de date folosit

Combinatia "alocare" + "copiere" este oarecum echivalenta cu new + constructorul de copiere din C++, "dealocare" este echivalent cu destructorul din C++.

In cazul structuri de date de tip "vector" se specifica dimensiunea tipului de date folosit. (nu este necesara o functie de alocare)

Lista dublu inlantuita.

Structuri:

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

Metode:

struct list * list_create(cb_allocator allocator, cb_deallocator deallocator, cb_data_copy data_copy);
- Creaza o lista goala

void list_delete(struct list *lst);
- Sterge toate elementele din lista + dealocare lista;


int list_get_size(struct list *lst);
- Intoarce dimensiunea listei;

void * list_pop_back(struct list *lst);
- Extrage ultimul element din lista (atentie: elemntul nu este dealocat, trebuie dealocat cand nu mai este folosit)

void list_push_back(struct list *lst, void *data);
- Adauga un element nou la sfarsitul listei

void * list_pop_front(struct list *lst);
- Extrage primul element din lista (atentie: elementul nu este dealocat, trebuie dealocat cand nu mai este folosit)

void list_push_front(struct list *lst, void *data);
- Adauga un element nou la inceputul listei

void * list_peek_front(struct list *lst);
- Intoarce adresa primului element din lista

void * list_peek_back(struct list *lst);
- Intoarge adresa ultimului element din lista

void * list_find(struct list *lst, cb_find test_fn, void *look_for);
- Cauta in lista un element "la fel" ca "look_for" d.p.d.v. a functiei de comparare "test_fn"

void list_iterate_data(struct list *lst, cb_iter iter_fn);
- Parcurge lista si pentru fiecare element din lista se apeleaza functia iter_fn

void list_iterate_nodes(struct list *lst, cb_iter iter_fn);
- Parcurge lista si pentru fiecare nod din lista se apeleaza functia iter_fn

void list_remove_node(struct list *lst, struct node *item);
- Sterge un nod din lista (implica dealocare elementului si a nodului)

struct node * list_get_first(struct list *lst);
- Intoarce adresa primului nod din lista

struct node * list_get_next(struct node *item);
- Intoarce adresa urmatorului nod al nodului item

struct node * list_get_last(struct list *lst);
- Intoarce adresa ultimului nod din lista

struct node * list_get_prev(struct node *item);
- Intoarce adresa nodului anterior nodului item


Stiva.

Structura:

struct stack
{
	struct list *dl_list;
};

Metode:
struct stack * stack_create(cb_allocator allocator, cb_deallocator deallocator, cb_data_copy data_copy)
- Creaza o stiva goala

void stack_delete(struct stack *n_stack)
- Dealoca elementele din stiva + eliberare stiva

void stack_push(struct stack *n_stack, void *data)
- Adauga un element nou in stiva

void * stack_pop(struct stack *n_stack)
- Extrage elementul de la varful stivei (atentie: elementul extras nu este dealocat)

void * stack_peek(struct stack *n_stack)
- Intoarce adresa elementului de la varful stivei

struct node * stack_get_top(struct stack *n_stack)
- Intoarce adresa nodului ce reprezinta varful stivei

struct node * stack_get_base(struct stack *n_stack)
- Intoarce adresa nodului ce reprezinta baza stivei

struct node * stack_get_prev(struct node *item)
- Intoarce adresa nodului anterior nodului "item"

struct node * stack_get_next(struct node *item)
- Intoarce adresa nodului urmator nodului "item"

int stack_get_size(struct stack *n_stack)
- Intoarce dimensiunea stivei

Vector.

Structura:

struct vector
{
	void *data;
	int element_size;
	int size;

	cb_allocator alloc;
	cb_deallocator dealloc;
	cb_data_copy data_copy;
};

Metode:

struct vector * vector_create(int element_size, cb_deallocator deallocator, cb_data_copy data_copy);
- Creaza o vector gol care contine elemente de dimensiune element_size

void vector_delete(struct vector *v);
- Sterge elementele din vector si dealoca vectorul

void * vector_item(struct vector *v, int idx);
- Intoarce elementul de pe pozitia idx

void * vector_end(struct vector *v);
- Intoarce adresa ultimului elemnt din vector

void vector_push_back(struct vector *v, void *data);
- Adauga un element nou in vector la sfarsitul acestuia

void vector_pop_back(struct vector *v);
- Sterge ultimul element din vector

void vector_erase(struct vector *v, int idx);
- Sterge elementul de pe pozitia idx

Implementare tema.

Variabile folosite:

struct list *lista_market;
- Contine lista produselor din "supermarket"; Aceasta lista este creata prin apelarea functiei "construieste_lista_produse_market"

struct list *lista_cumparaturi;
- Contine produsele din lista de cumparaturi; Aceasta lista este creata in cazul cerintei 1 prin apelarea functiei "construieste_lista_cumparaturi". In cazul in care cerinta 2 este activa, aceasta lista este filtrata prin eliminarea produselor din lista de cumparaturi care au stoc zero in lista_market.

struct list *produse_cumparate;
- Contine produsele cumparate in cazul cerintei 3 (cumpara_cu_buget) sau cerintei 4 (cumparar_fara_limite), in cazul in care cerintele 3 si 4 sunt active, se dealoca lista inaintea executarii cerintei 4.

struct stack *stiva_compara_preturi;
struct vector *vector_compara_preturi
- Cerinta 6 (din enuntul acesteia...) necesita folosirea unei stive si a unui vector; Se construieste stiva "stiva_compara_preturi" conform algoritmului din enunt (creeaza_stiva_preturi) si elementele acestei stive se introduc in vectorul "vector_compara_preturi" (creeaza_vector_din_stiva) prin parcurgerea stivei de la baza la varf.

Variabile globale:
output - FILE * (handle catre fisierul rezultate)
cerinte - unsigned (este initializat prin rularea functiei parse_rules)
	  codificare "cerinte": puteri ale lui 2
Identificare cerinta:
enum {
	CERINTA1 = 0x01,
	CERINTA2 = 0x02,
	CERINTA3 = 0x04,
	CERINTA4 = 0x08,
	CERINTA5 = 0x10,
	CERINTA6 = 0x20
};

cerinte & CERINTA{1...6};

buget - int (initializat in urma executarii cerintei 2)

Structura produs supermarket (market_item.h):

struct market_item
{
	char *nume;
	char *categorie;
	int pret;
	int cantitate;
	int stoc;
};

Functii necesare pentru folosirea structurii "market_item" in cadrul listei "lista_market"
void * market_allocator()
void market_deallocator(void *data)
void market_copy(void *src, void *dst)


Structura produs de cumparat (buy_item.h):

struct buy_item
{
	char *nume;
	char *categorie;
	int cantitate;
};

Functii necesare pentru folosirea structurii "buy_item" in cadrul listei "lista_cumparaturi, produse_cumparate"
void * buy_item_allocator()
void buy_item_deallocator(void *data)
void buy_item_copy(void *src, void *dst)

Structura pentru cerinta 6 (element.h): 

struct element
{
	int val;
};

Functii necesare pentru folosirea structurii "element" in cadrul stivei/vectorului "stiva_compara_preturi, vector_compara_preturi"

void * element_allocator() (nu este necesara pentru vector)
void element_copy(void *src, void *dst)
void element_deallocator(void *data)



Modul in care am implementat structurile de date (lista, stiva, vector) elimina cazurile de "scurgere de memorie" (daca operatiile asupra acestor tipuri de date au fost implementate in mod corect). Mentiune: prin constructie, functiile list_pop_{front/back}, stack_pop nu dealoca zona de memorie aferenta elementului extras, acest lucru revine in sarcina "programatorului". Pentru orice {list/stack/vector}_create trebuie sa existe un apel catre {list/stack/vector}_delete atunci cand aceste structuri nu mai sunt folosite.

--- malloc vs calloc (alocare structuri/initializare) - Pentru structuri de dimensiuni relativ "mici" este de preferat alocarea memoriei si intializare acesteia cu zero. Codul este mai simplu, si, faciliteaza detectarea erorilor de programare inca din faza de testare (spre exemplu null pointer de-referencing).

Output checker.sh:

./checker.sh 
gcc -I. -Wall   -c -o market.o market.c
gcc -I. -Wall   -c -o dl_list.o dl_list.c
gcc -I. -Wall   -c -o vector.o vector.c
gcc -o market market.o dl_list.o vector.o -I. -Wall
-------------------------------------------------------------
Executabil = 'market'
-------------------------------------------------------------
[TESTE NORMALE]
Test  1 ................................. PASSED (+ 7pt)
Test  2 ................................. PASSED (+ 7pt)
Test  3 ................................. PASSED (+ 7pt)
Test  4 ................................. PASSED (+ 7pt)
Test  5 ................................. PASSED (+ 7pt)
Test  6 ................................. PASSED (+ 7pt)
Test  7 ................................. PASSED (+ 7pt)
Test  8 ................................. PASSED (+ 7pt)
Test  9 ................................. PASSED (+ 7pt)
Test 10 ................................. PASSED (+ 7pt)
Test 11 ................................. FAILED (+ 0pt)
Test 12 ................................. FAILED (+ 0pt)
Test 13 ................................. FAILED (+ 0pt)
Test 14 ................................. PASSED (+ 5pt)
Test 15 ................................. PASSED (+ 5pt)
Test 16 ................................. PASSED (+ 5pt)

TOTAL ...........................................  85pt

-------------------------------------------------------------
[NOTA FINALA]

NOTA FINALA .....................................  85pt
