#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "array.h"
#include "scanner.h"


struct dictionary 
{
    Array *array;
};

struct keyvalue
{
    char *key;
    void *value;
};

Dictionary *newDictionary()
{
    Dictionary *d = allocate(sizeof(Dictionary));
    d->array = newArray();
    return d;
}

int containsKey(Dictionary *d, char *k)
{
    Array *a = d->array;
    int i;
    for(i = 0; i < getArraySize(a); i++)
    {

        KeyVal *kvf = getIndex(a, i);
        if(strcmp(kvf->key, k) == 0)
            return 1;
    }
    return 0;
}

void *getValue(Dictionary *d, char *k)
{
    Array *a = d->array;
    int i;
    for(i = 0; i < getArraySize(a); i++)
    {

        KeyVal *kvf = getIndex(a, i);
        if(strcmp(kvf->key, k) == 0)
            return  kvf->value;
    }
    return NULL;
}

int compare(KeyVal *one, KeyVal *two)
{
    return strcmp(one->key, two->key);
}

int split(Array *a, int low, int high)
{
    KeyVal *pivot = getIndex(a, high);
    int i = (low - 1);
    int j;
    KeyVal *r;
    KeyVal *l;
    for(j = low; j < high; j++)
    {
        l = getIndex(a, j);
        if(compare(l, pivot) < 0)
        {
            i++;
            r = getIndex(a, i);
            setArray(a, j, r);
            setArray(a, i, l);
        }
    }
    l = getIndex(a, i+1);
    r = getIndex(a, high);
    setArray(a, i+1, r);
    setArray(a, high, l);
    return (i + 1);
}

void sortDic(Array *a, int low, int high)
{
    if(low < high)
    {
        int p = split(a, low, high);
        sortDic(a, low, p-1);
        sortDic(a, p+1, high);
    }
}

void displayDictionary(Dictionary *d, FILE*fp)
{
    Array *a = d->array;
    int i;
    for(i = 0; i < getArraySize(a); i++)
    {
        KeyVal *kvf = getIndex(a, i);
        char *v = kvf->value;
        fprintf(fp,"%s: %s ", kvf->key, v);
    }
    return;
}

void addKVF(Dictionary *d, char *k, void *v)
{
    KeyVal *kvf = allocate(sizeof(KeyVal));
    kvf->key = k;
    kvf->value = v;
    append(d->array, kvf);
    sortDic(d->array, 0, getArraySize(d->array) - 1);
    return;
}
