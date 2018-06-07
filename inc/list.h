#ifndef LIST_H
#define LIST_H

#include "types.h"

typedef struct list_elem {
    struct list_elem *prev;
    struct list_elem *next;
}list_elem;

typedef struct  list
{
    struct list_elem head;
    struct list_elem tail;
}list;

typedef bool function(list_elem *elem, int arg);

void list_init(list *list);
void list_push(list *list, list_elem *elem);
void list_append(list *list, list_elem *elem);
void list_remove(list_elem *elem);
list_elem *list_pop(list *list);
bool elem_find(list *list, list_elem *elem);
list_elem *list_traversal(list *list, function func, int arg);
uint32_t list_len(list *list);
bool list_empty(list *list);


#endif
