#include <stdlib.h>

typedef struct array Array;

Array *newArray(void);
void append(Array *, void *);
void *getIndex(Array *, int);
int getArraySize(Array *);
void freeArray(Array *);
void setArray(Array *, int, void *);

