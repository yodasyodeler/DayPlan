#include "c_list.h"
#include <stdlib.h>

/*  add
 *    Appends a node to the list
 */
void addNode(node** list, void* data)
{
	if ((*list) == NULL){
		(*list) = (node*) malloc(sizeof(node));
		(*list)->_data = data;
		(*list)->_next = NULL;
	}
	else
		addNode( (&(*list)->_next), data);
}

/*  remove
 *    removes a node that _data meets the requirements of the compare function
 *    returns:
 *      1: no match to data
 *      0: node deleted successfully
 */
int removeNode(node** list, const void* data, int (*comp)(void* data1, const void* data2), void (*delete)(void* data))
{
	int re = 0;
	node* temp = NULL;

	if ((*list) == NULL)
		re = 1;
	else if (comp((*list)->_data, data) == 0){
		temp = (*list)->_next;
		delete( (*list)->_data );
		free( (*list)->_data );
		free( (*list));
		(*list) = temp;
	}
	else
		re = removeNode( &((*list)->_next), data, comp, delete );

	return re;
}

/*  search
 *    returns node that matches comp
 */
node* searchNode(node* list, const void* data, int (*comp)(void* data1, const void* data2))
{
	int num = comp(list->_data, data );
	if (list != NULL && num)
		return searchNode(list->_next, data, comp);
	else
		return list;
}

/*  purge
 *    deletes all nodes
 */
void purgeNode(node** list, void (*delete)(void* data))
{
	node* temp;
	if (list != NULL){
		temp = (*list)->_next;
		delete((*list)->_data);
		free( (*list)->_data );
		free( (*list));
		*list = NULL;
		purgeNode(&(temp), delete);
	}
}
