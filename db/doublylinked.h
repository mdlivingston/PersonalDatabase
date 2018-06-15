#include "node.h"

typedef struct list List;

List *newList(void);
void addListNode(List *, void *);
void *peekListHead(List *);
void *peekListTail(List *);
void removeListHead(List *);
void removeListTail(List *);
void *iterateList(List *);
void printList(List *list);
void freeList(List *);

