#include "doublylinked.h"
#include "node.h"
#include "scanner.h"

void printInt(int data)
{
    printf("%d ", data);
}

void printNewline(void)
{
    printf("\n");
}

struct list
{
    Node *head;
    Node *tail;
    Node *current;
};

List *newList(void)
{
    List *list = allocate(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    return list;
}

void addListNode(List *list, void *data)
{
    Node *node = newNode();
    setNodeData(node, data);
    if(list->head == NULL)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        setNodePrevious(node, list->tail);
        setNodeNext(list->tail, node);
        list->tail = node;
    }
}

void *peekListHead(List *list)
{
    if(list->head == NULL)
        return NULL;
    return getNodeData(list->head);
}

void *peekListTail(List *list)
{
    if(list->head == NULL)
        return NULL;
    return getNodeData(list->tail);
}

void removeListHead(List *list)
{
    Node *current = list->head;
    setNodePrevious(getNodeNext(list->head), NULL);
    list->head =  getNodeNext(list->head);
    free(current);
}

void removeListTail(List *list)
{
    Node *current = list->tail;
    list->tail = getNodePrevious(list->tail);
    setNodeNext(list->tail, NULL);
    free(current);
}

void *iterateList(List *list)
{
    list->current = list->current != NULL ? getNodeNext(list->current) : list->head;
    return getNodeData(list->current);
}

void printList(List *list)
{
    Node *current = list->head;
    while(current != NULL)
    {
        printInt(*(int *)getNodeData(current));
        current = getNodeNext(current);
    }
    printNewline();
}

void freeList(List *list)
{
    Node *current = list->head;
    while(current != NULL)
    {
        freeNode(current);
        current = getNodeNext(current);
    }
    free(list);
}

