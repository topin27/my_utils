#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>

#include "hashtable.h"


typedef struct _ListNode {
	NodeInfo nodeinfo;
	ListHead list;
} ListNode;

static ListHead *hashtable = NULL;
static int hashtable_size = 0;

int hashtable_init(ListHead *table, int size)
{
	int i;

	assert(size > 0);

	hashtable = table;
	hashtable_size = size;

	for (i = 0; i < size; i++)
		list_head_init(&hashtable[i]);

	return 0;
}

static unsigned long time33(const char *str, int len)
{
	unsigned long hash = 0;
	int i;

	for (i = 0; i < len; i++)
		hash = (hash << 5) + hash + (unsigned long )str[i];
	
	return hash;
}

NodeInfo *hashtable_getitem(const char *str, unsigned code)
{
	unsigned long index;
	ListHead *plist;

	index = time33(str, strlen(str)) % hashtable_size;
	LIST_FOR_EACH(plist, &hashtable[index]) {
		ListNode *node = LIST_ENTRY(plist, ListNode, list);

		if (strcmp(node->nodeinfo.str, str) == 0
		    && node->nodeinfo.code == code) {
			return &node->nodeinfo;
		}
	}

	return NULL;
}

int hashtable_getindex(const char *str, unsigned code)
{
	unsigned long index;
	ListHead *plist;

	index = time33(str, strlen(str)) % hashtable_size;
	LIST_FOR_EACH(plist, &hashtable[index]) {
		ListNode *node = LIST_ENTRY(plist, ListNode, list);

		if (strcmp(node->nodeinfo.str, str) == 0 
		    && node->nodeinfo.code == code) {
			return index;
		}
	}

	return -1;
}

int hashtable_add(const NodeInfo *node_info)
{
	int index;

	index = hashtable_getindex(node_info->str, node_info->code);
	if (index < 0) {
		ListNode *node = (ListNode *)malloc(sizeof(*node));

		memcpy(&node->nodeinfo, node_info, sizeof(*node_info));

		index = time33(node->nodeinfo.str, 
		               strlen(node->nodeinfo.str))
			       % hashtable_size;
		list_add(&node->list, &hashtable[index]);
	} 
	
	return index;
}

int hashtable_del(const NodeInfo *node_info)
{
	int index = -1;

	index = hashtable_getindex(node_info->str, node_info->code);
	if (index < 0) {
		return -1;
	} else {
		ListHead *tmp, *plist;

		for (plist = hashtable[index].next; 
		     plist != &hashtable[index]; ) {
			ListNode *node = LIST_ENTRY(plist, ListNode, list);

			tmp = plist->next;
			if (strcmp(node_info->str, node->nodeinfo.str) == 0
			    && node->nodeinfo.code == node_info->code) {
				list_del(plist);
				free(node);
				node = NULL;
				break;
			}
			plist = tmp;
		}
	}
		
	return index;
}

void hashtable_clean(void)
{
	int i = 0, item_count = 0, total_count = 0;
	ListHead *plist, *tmp;

	if (hashtable_size == 0) return;

	for (i = 0; i < hashtable_size; i++) {
		if (hashtable[i].next == &hashtable[i]) 
			continue;
		
		item_count++;
		for (plist = hashtable[i].next; plist != &hashtable[i];) {
			ListNode *node = LIST_ENTRY(plist, ListNode, list);
			
			tmp = plist->next;
			list_del(plist);
			free(node);
			node = NULL;
			plist = tmp;
			total_count++;
		}
	}
}

void hashtable_exit(void)
{
	hashtable_clean();
}
