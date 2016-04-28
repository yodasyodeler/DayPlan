#ifndef LIST_H_
#define LIST_H_

typedef struct _node{
	struct _node* _next;
	void*  _data;
}node;

void addNode(node** list, void* data);
int removeNode(node** list, const void* data, int (*comp)(void* data1, const void* data2), void (*delete)(void* data));
node* searchNode(node* list, const void* data,int (*comp)(void* data1, const void* data2));
void purgeNode(node** list, void (*delete)(void* data));

#endif
