#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemblage.h"
#include "document.h"
#include "scanner.h"
#include "array.h"
#include "doublylinked.h"

struct Assemblage
{
    char *name;
    FILE *fp;
    List *documents;
};

Assemblage *newAssemblage(char *n, FILE *f)
{
    Assemblage *c = allocate(sizeof(Assemblage));
    c->name = n;
    c->fp = f;
    c->documents = newList();
    return c;
}

char *getAssemblageName(Assemblage *c)
{
    return c->name;
}

Array *parseCondition(char *c, char *s)
{
    char fd[777];
    char op[777];
    char val[777];
    int position = 0;
    int i;
    char charred;
    int j = 0;
    Array *arr = newArray();
    
    for(i = 0; i < strlen(c); i++){
        charred = c[i];
        if(position == 0){
            if(charred == '=' || charred == '>' || charred == '<'){
                fd[j] = '\0';
                position = 1;
                j = 0;
                op[j] = charred;
                j++;
            }
            else{
                fd[j] = charred;
                j++;
            }
        }
        else if(position == 1){
            if(charred != '>' && charred != '<' && charred != '='){
                op[j] = '\0';
                j = 0;
                position = 2;
                val[j] = charred;
                j++;
            }
            else{
                op[j] = charred;
                j++;
            }
        }
        else if(position == 2){
            val[j] = charred;
            j++;
        }
    }
    val[j] = '\0';
    append(arr, s);
    append(arr, strdup(fd));
    append(arr, strdup(op));
    append(arr, strdup(val));
    return arr;
}
int hasAnyField(Document *d, List *conditions)
{
    Array *current;
    char *f;
    int hasFd = 0;
    
    current = iterateList(conditions);
    while(current != NULL)
    {
        f = getIndex(current, 1);
        if(hasF(d, f))
            hasFd = 1;
        current = iterateList(conditions);
    }
    return hasFd;
}
int isMet(Document *d, char *f, char *x, char *v)
{
    int val = atoi(v);
    int vl;
    if(hasF(d, f))
    {
        vl = getFieldValue(d, f);
    }
    else
        return 1;
    if(strcmp(x, "=") == 0)
        return (vl == val);
    if(strcmp(x, ">=") == 0)
        return (vl >= val);
    if(strcmp(x, "<=") == 0)
        return (vl <= val);
    if(strcmp(x, "<") == 0)
        return (vl < val);
    if(strcmp(x, ">") == 0)
        return (vl > val);
    if(strcmp(x, "<>") == 0)
        return (vl != val);
    return 0;
}
Array *isMets(Array *all, List *conds)
{
    Array *new = newArray();
    int i;
    int meetsAll = 1;
    Array *current;
    for(i = 0; i < getArraySize(all); i++)
    {
        Document *d = getIndex(all, i);
        if(hasAnyField(d, conds))
        {
            current = iterateList(conds);
            while(current != NULL) {
                int mc = isMet(d, getIndex(current, 1), getIndex(current, 2), getIndex(current, 3));
                if(!mc)
                    meetsAll = 0;
                current = iterateList(conds);
            }
            if(meetsAll)
            {
                append(new, d);
            }
        }
        meetsAll = 1;
    }
    return new;
}



Array *getCondition(char *cond, int s)
{
    char c[255];
    char str[255];
    int i;
    int j = 0;
    char ch;
    
    for(i = s; i < strlen(cond); i++)
    {
        ch = cond[i];
        if(ch == ' ') continue;
        else if(ch == ',')
        {
            j++;
            break;
        }
        else
        {
            c[j] = ch;
            j++;
        }
    }
    c[j] = '\0';
    sprintf(str, "%d", i+1);
    return parseCondition(strdup(c), strdup(str));
}

Array *fieldFilter(Assemblage *c, char *f)
{
    Array *a = newArray();
    Document *current = iterateList(c->documents);
    while(current != NULL)
    {
        if(hasF(current, f))
            append(a, current);
        current = iterateList(c->documents);
    }
    return a;
}

Array *latestFilter(Assemblage *c, Array *a)
{
    Array *new = newArray();
    int i;
    for(i = 0; i < getArraySize(a); i++)
    {
        Document *d = getIndex(a, i);
        if(isLatest(d))
        {
            append(new, d);
        }
    }
    return new;
}

Array *versionNumberFilter(Assemblage *c, Array *a, int v)
{
    int i;
    int j;
    Document *doc1;
    Document *doc2;
    Array *new = newArray();
    Array *latest = latestFilter(c, a);
    for(i = 0; i < getArraySize(a); i++)
    {
        doc1 = getIndex(a, i);
        if(isLatest(doc1))
        {
            append(new, doc1);
        }
        else {
            for(j = 0; j < getArraySize(latest); j++)
            {
                doc2 = getIndex(latest, j);
                if(getDocID(doc1) == getDocID(doc2))
                {
                    if(getDocVersion(doc1) > (getDocVersion(doc2) - v))
                    {
                        append(new, doc1);
                    }
                }
            }
        }
    }
    return new;
}

Array *condtitionFilter(Assemblage *c, char *conditions)
{
    int moreConditions = 1;
    int s = 0;
    Array *arr = newArray();
    List *conds = newList();
    Document *current = iterateList(c->documents);
    while(current != NULL)
    {
        append(arr, current);
        current = iterateList(c->documents);
    }
    if(strlen(conditions) == 0)
        return arr;
    else
    {
        while(moreConditions)
        {
            Array *cond = getCondition(conditions, s);
            addListNode(conds, cond);
            char *stopped = getIndex(cond, 0);
            s = atoi(stopped);
            if(strlen(conditions) <= s)
                moreConditions = 0;
        }
        return isMets(arr, conds);
    }
}





int getVNumber(List *docs, Document *d)
{
    int version = 1;
    Document *current = iterateList(docs);
    while(current != NULL)
    {
        if(getDocID(current) == getDocID(d))
        {
            setDocLatest(current, 0);
            ++version;
        }
        current = iterateList(docs);
    }
    return version;
}


void addDoc(Assemblage *c, Document *d)
{
    setDocVersion(d, getVNumber(c->documents, d));
    setDocLatest(d, 1); 
    addListNode(c->documents, d);
}

void showDocs(Assemblage *c, FILE *fp)
{
    Document *current = iterateList(c->documents);
    while(current != NULL)
    {
        showDocument(current, fp);
        printf("\n");
        current = iterateList(c->documents);
    }
    return;
}
