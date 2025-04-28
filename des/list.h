#ifndef C_LIST_H
#define C_LIST_H

typedef struct list
{
	int			 size;
	struct node* head;
	struct node* tail;
} list_t;

typedef struct node
{
	unsigned long data;
	struct node*  next;
	struct node*  prev;
} node_t;

typedef struct single_in_out
{
	int			  size;
	struct list** in_xor;
} single_in_out_s;

list_t* list_init(list_t* list);
int		list_append(list_t* list, unsigned long data);
int		list_print(list_t* list);

single_in_out_s* single_in_out_init(single_in_out_s* single);

#endif
