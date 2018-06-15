
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "document.h"
#include "dictionary.h"
#include "scanner.h"

struct document
{
  int version;
  int sysID;
  int DocID;
  int latest;
  Dictionary *fields;
};

Document *newDocument(int sysid) 
{
    Document *d = allocate(sizeof(Document));
    d->sysID = sysid;
    d->fields = newDictionary();
    return d;
}

int getDocVersion(Document *d)
{
    return d->version;
}

int getDocID(Document *d)
{
    return d->DocID;
}

int getSysID(Document *d)
{
    return d->sysID;
}

void setDocVersion(Document *d, int v)
{
    d->version = v;
    return;
}

void setDocLatest(Document *d, int l)
{
    d->latest = l;
    return;
}

int isLatest(Document *d)
{
    return d->latest;
}

int hasF(Document *d, char *f)
{
    if(containsKey(d->fields, f))
        return 1;
    return 0;
}

int getFieldValue(Document *d, char *f)
{
    char *v = getValue(d->fields, f);
    if(v != NULL)
        return atoi(v);
    return -1;
}

void addField(Document *d, char *k, char *v)
{
    if(strcmp(k, "DocID") == 0)
    {
        int val = atoi(v);
        d->DocID = val;
    }
    else
    {
        addKVF(d->fields, strdup(k), strdup(v));
    }
    return;
}

void showDocument(Document *d, FILE*fp)
{
    fprintf(fp,"vn: %d ", d->version);
    fprintf(fp,"sysid: %d ", d->sysID);
    fprintf(fp,"DocID: %d ", d->DocID);
    displayDictionary(d->fields, fp);
    fprintf(fp,"\n");
}

Document *createDoc(char *l, int sysid, int start)
{
    Document *d = newDocument(sysid);
    char v[255];
    char k[255];
    
    char ch;
    int i; 
    int j = 0;
    int onKey = 1;
    for(i = start; i<strlen(l); i++)
    {
        ch = l[i];
        if(ch == ':') {
            onKey = 0;
            k[j] = '\0';
            j = 0;
        }
        else if(ch == ' '){
            v[j] = '\0';
            addField(d, k, v);
            memset(&k[0], 0, sizeof(k));
            memset(&v[0], 0, sizeof(v));
            onKey = 1;
            j = 0;
        }
        else if(onKey){
            k[j] = l[i];
            ++j;
        }
        else{
            v[j] = ch;
            ++j;
        }
    }
    addField(d, k, v);
    return d;
}
