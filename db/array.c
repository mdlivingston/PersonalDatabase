
#include <stdlib.h>
#include <stdio.h>

#include "array.h"
#include "scanner.h"

#define DEFAULT_SIZE 8 

struct array
{
    void **array;
    int size;
    int currentIndex;
};

Array *newArray()
{
    Array *a = allocate(sizeof(Array));
    a->array = allocate(sizeof(void *) * DEFAULT_SIZE);
    a->size = DEFAULT_SIZE;
    a->currentIndex = 0;
    return a;
}

void freeArray(Array *a)
{
    free(a->array);
    free(a);
}

void append(Array *arr, void *item)
{
    int index = arr->currentIndex;
    if(index == arr->size)
    {
        arr->size = arr->size * 2;
        arr->array = reallocate(arr->array, sizeof(void *) * arr->size);
    }
    arr->array[index] = item;
    arr->currentIndex += 1;
}

void *getIndex(Array *a, int index)
{
    return a->array[index];
}

int getArraySize(Array *a)
{
    return a->currentIndex;
}

void setArray(Array *a, int index, void *v)
{
    a->array[index] = v;
}
