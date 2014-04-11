/*
 * list template, implement by C. Just little modification of Linux kernel's list.
 * 
 * +------+
 * | prev |------------------------------------------------------------------------------------------------------------+
 * +------+            +-------------------+            +-------------------+            +-------------------+         |
 * | next | <-----------+                  |            |                   |            |                   |         |
 * +------+            | \   ( val )       |            |      ( val )      |            |      ( val )      |         |
 *    ^  \             |  \                |            |                   |            |                   |         |
 *    |   \            |   \               |            |                   |            |                   |         |
 *    |    \           |    \              |            |                   |            |                   |         |
 *    |     \          |     \             |            |                   |            |                   |         |
 *    |      \         |    +------+       |            |      +------+     |            |      +------+     |         |
 *    |       \        |    | prev |<--------------------------| prev |<------------------------| prev |<--------------+
 *    |        +----------->+------+       |            |      +------+     |            |      +------+     |
 *    |                |    | next |-------------------------->| next |------------------------>| next |---------------+
 *    |                |    +------+       |            |      +------+     |            |      +------+     |         |
 *    |                +-------------------+            +-------------------+            +-------------------+         |
 *    |                                                                                                                |
 *    |                                                                                                                |
 *    +----------------------------------------------------------------------------------------------------------------+
 *
 * === EXAMPLE:

	#include <stdio.h>
	#include "list.h"
	
	typedef struct _IntNode {
		int val;
		ListHead list;
	} IntNode;
	
	int main(int argc, char *argv[])
	{
		ListHead head, *plist;
		IntNode a, b;
	
		a.val = 2;
		b.val = 3;
	
		list_head_init(&head);
		list_add(&a.list, &head);
		list_add(&b.list, &head);
	
		LIST_FOR_EACH(plist, &head) {
			IntNode *node = LIST_ENTRY(plist, IntNode, list);
			printf("var=%d\n", node->val);
		}
	
		return 0;
	}

 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct _ListHead {
	struct _ListHead *next, *prev;
} ListHead;

#define OFFSET_OF(Type, member) ((unsigned long)&((Type *)0)->member)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.  */
#define CONTAINER_OF(ptr, Type, member) (Type *)((char *)ptr - OFFSET_OF(Type, member))

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.  */
#define LIST_POISON1  ((void *)0x00100100)
#define LIST_POISON2  ((void *)0x00200200)

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.  */
#define LIST_ENTRY(ptr, Type, member)  CONTAINER_OF(ptr, Type, member)

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	ListHead name = LIST_HEAD_INIT(name)

//static inline void INIT_LIST_HEAD(struct list_head *list)
static inline void list_head_init(ListHead *list)
{
	list->next = list;
	list->prev = list;
}

/**
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.  */
#define LIST_FOR_EACH(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_r    -    iterate over a list reversely
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.  */
#define LIST_FOR_EACH_R(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)    

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!  */
static inline void __list_add(ListHead *new,
                              ListHead *prev,
                              ListHead *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.  */
static inline void list_add(ListHead *new, ListHead *head)
{
	__list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.  */
static inline void list_add_tail(ListHead *new, ListHead *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!  */
static inline void __list_del(ListHead *prev, ListHead *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.  */
static inline void list_del(ListHead *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}


/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.  */
static inline int list_is_empty(const ListHead *head)
{
	return head->next == head;
}


static inline void __list_splice(ListHead *list,
                                 ListHead *head)
{
	ListHead *first = list->next;
	ListHead *last = list->prev;
	ListHead *at = head->next;
	
	first->prev = head;
	head->next = first;
	
	last->next = at;
	at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.  */
static inline void list_splice(ListHead *list, ListHead *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

#endif
