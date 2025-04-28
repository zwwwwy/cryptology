#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list_t* list_init(list_t* list)
{
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

int list_append(list_t* list, unsigned long data)
{
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	new_node->data	 = data;
	new_node->prev	 = list->tail;
	new_node->next	 = NULL;

	if (list->size == 0)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		list->tail->next = new_node;
		list->tail		 = new_node;
	}
	list->size++;
	return 0;
}

int list_print(list_t* list)
{
	node_t* current = list->head;
	while (current != NULL)
	{
		printf("%.6lb | ", current->data);
		current = current->next;
	}
	printf("\n");
	return 0;
}

single_in_out_s* single_in_out_init(single_in_out_s* single)
{
	single->size   = 16;
	single->in_xor = (list_t**)malloc(sizeof(list_t*) * single->size);
	for (int i = 0; i < single->size; ++i)
	{
		single->in_xor[i] = (list_t*)malloc(sizeof(list_t));
		list_init(single->in_xor[i]);
	}
	return single;
}
