#include "list.h"
#include "idt.h"

void list_init(list *list)
{   
	list->head.prev = NULL;
	list->tail.next = NULL;
	list->head.next = &list->tail;
	list->tail.prev = &list->head;
}

static void list_insert_before(list_elem *before, list_elem *elem)
{
	enum intr_status old_status = intr_disable();

	before->prev->next = elem;
	elem->prev = before->prev;
	elem->next = before;
	before->prev = elem;

	intr_set_status(old_status);
}

void list_push(list *list, list_elem *elem)
{
	list_insert_before(list->head.next, elem);
}

void list_append(list *list, list_elem *elem)
{
	list_insert_before(&list->tail, elem);
}

void list_remove(list_elem *elem)
{
	enum intr_status old_status = intr_disable();

	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;

    intr_set_status(old_status);
}

list_elem *list_pop(list *list)
{
	list_elem *elem = list->head.next;
	list_remove(elem);
	return elem;
}

bool elem_find(list *list, list_elem *obj_elem)
{
	list_elem *elem = list->head.next;

	while(elem != &list->tail) {
		if(elem == obj_elem) {
			return TRUE;
		}
		elem = elem->next;
	}

	return FALSE;
}

list_elem *list_traversal(list *list, function func, int arg)
{
	list_elem *elem = list->head.next;

	if(elem == &list->tail)
		return NULL;

	while(elem != &list->tail)
	{
		if(func(elem, arg))
			return elem;
		elem = elem->next;
	}

	return NULL;
}

uint32_t list_len(list *list)
{
    list_elem *elem = list->head.next;
    uint32_t length = 0;

    while(elem != &list->tail)
    {
    	length++;
    	elem = elem->next;
    }

    return length;
}

bool list_empty(list *list)
{
	if(list->head.next == &list->tail)
		return TRUE;
	
	return FALSE;
}
