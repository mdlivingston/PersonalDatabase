

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "assemblage.h"
#include "document.h"
#include "doublylinked.h"
#include "array.h"
#include "scanner.h"

struct database
{
    char *name;
    int sysid;
    List *Assemblages;
};

Database *newDatabase(char *n)
{
    Database *d = allocate(sizeof(Database));
    d->name = n;
    d->sysid = 0;
    d->Assemblages = newList();
    return d; 
}

int getDBSysID(Database *d)
{
    return d->sysid;
}

void incSId(Database *d)
{
    d->sysid = d->sysid + 1;
    return;
}

void addAssemblage(Database *d, Assemblage *c)
{
    addListNode(d->Assemblages, c);
}

Assemblage *getAssemblage(Database *d, char *n)
{
    Assemblage *current = iterateList(d->Assemblages);
    Assemblage *c = current;
    while(current != NULL)
    {
        if(strcmp(getAssemblageName(current), n) == 0)
        {
            c = current;
        }
        current = iterateList(d->Assemblages);
    }
    return c;
}

void showAssemblages(Database *d, FILE* fp)
{
    Assemblage *current = iterateList(d->Assemblages);
    while(current != NULL)
    {
        printf("Assemblage: %s\n\n", getAssemblageName(current));
        showDocs(current, fp);
        printf("\n\n");
        current = iterateList(d->Assemblages);
    }
    return;
}

Array *braceValues(char *l, int s)
{
    char field[255];
    char str[255];
    int i;
    int j = 0;
    char ch;
    Array *a = newArray();

    for(i = s; i < strlen(l); i++)
    {
        ch = l[i];
        if(ch == '[') continue;
        else if(ch == ']')
        {
            field[j] = '\0';
            break;
        }
        else
        {
            field[j] = ch;
            j++;
        }
    }
    append(a, strdup(field));
    sprintf(str, "%d", i+1);
    append(a, strdup(str));
    return a;
}

int partByField(Array *a, char *f, int low, int high)
{
    Document *pivot = getIndex(a, high);
    int i = (low - 1);
    int j;
    Document *l;
    Document *r;

    for(j = low; j < high; j++)
    {
        l = getIndex(a, j);
        if(getFieldValue(pivot, f) > getFieldValue(l, f))
        {
            i++;
            r = getIndex(a, i);
            setArray(a, j, r);
            setArray(a, i, l);
        }
    }
    l = getIndex(a, i+1);
    r = getIndex(a, high);
    setArray(a,i+1, r);
    setArray(a, high, l);
    return (i + 1);
}

void sortByField(Array *a, char *f, int low, int high)
{
    if(low < high)
    {
        int p = partByField(a, f, low, high);
        sortByField(a, f, low, p-1);
        sortByField(a, f, p+1, high);
    }
}

int partByDocID(Array *a, int low, int high)
{
    Document *pivot = getIndex(a, high);
    int i = (low - 1);
    int j;
    Document *l;
    Document *r;

    for(j = low; j < high; j++)
    {
        l = getIndex(a, j);
        if(getDocID(pivot) > getDocID(l))
        {
            i++;
            r = getIndex(a, i);
            setArray(a, j, r);
            setArray(a, i, l);
        }
    }
    l = getIndex(a, i+1);
    r = getIndex(a, high);
    setArray(a,i+1, r);
    setArray(a, high, l);
    return (i + 1);
}

void sortByDocID(Array *a, int low, int high)
{
    if(low < high)
    {
        int p = partByDocID(a, low, high);
        sortByDocID(a, low, p-1);
        sortByDocID(a, p+1, high);
    }
}

void sortByVersion(Array *a, int n)
{
    int i, j;
    Document *d1;
    Document *d2;
    for(i = 1; i < n; i++)
    {
        d1 = getIndex(a, i);
        j = i - 1;
        d2 = getIndex(a, j);
        while(j >= 0 && (getDocVersion(d1) > getDocVersion(d2)) && (getDocID(d1) == getDocID(d2)))
        {
            setArray(a, j+1, d2);
            j = j-1;
            d2 = getIndex(a, j);
        }
        setArray(a, j+1, d1);
    }
}

int skipComma(char *l, int s)
{
    int i;
    for(i = s; i < strlen(l); i++) 
    {
        if(l[i] == ',' || l[i] == ' ') 
            continue;
        else
            break;
    }
    return i;
}

char *getVersionSection(char *l, int s)
{
    char versions[255];
    int i;
    int j = 0;
    char ch;
    int hasBracket = 0;
    int modifiedVersions = 0;

    s = skipComma(l, s);

    for(i = s; i < strlen(l); i++)
    {
        ch = l[i];
        if(ch == '[')
        {
            hasBracket = 1;
            continue;
        }
        else if(ch == ']')
        {
            versions[j] = '\0';
            break;
        }
        else
        {
            versions[j] = ch;
            modifiedVersions = 1;
            j++;
        }

    }
    if(hasBracket == 0)
        return "latest";
    else if(modifiedVersions == 0)
        return "all";
    return strdup(versions);
}

Array *filterVersions(Array *a, Assemblage *c, char *l, int s)
{
    char *versionSection = getVersionSection(l, s);
    if(strcmp(versionSection, "all") == 0);
    else if(strcmp(versionSection, "latest") == 0)
        a = latestFilter(c, a);
    else
        a = versionNumberFilter(c, a, atoi(versionSection));
    return a;
}

Array *getFieldsArr(char *f)
{
    Array *new = newArray();
    int i;
    int j = 0;
    char ch;
    char field[255];
    
    for(i = 0; i < strlen(f) + 1; i++)
    {
        ch = f[i];
        if(ch == ' ') continue;
        if(ch == ',')
        {
            field[j] = '\0';
            j = 0;
            append(new, strdup(field));
        }
        else
        {
            field[j] = ch;
            j++;
        }
    }
    field[j] = '\0';
    append(new, strdup(field));
    return new;
}

void displayFilteredResults(Array *a, Array *f, FILE*fp)
{
   // fprintf(f, "Some text: %s\n", text);
    int i;
    int j;
    char *charred;
    int dead = 0;

    sortByVersion(a, getArraySize(a));
    for(i = 0; i < getArraySize(a); i++){
        Document *d = getIndex(a, i);
        for(j = 0; j < getArraySize(f); j++){
            char *charred2 = getIndex(f, j);
            if(hasF(d, charred2) || strcmp(charred2, "DocID") == 0 || strcmp(charred2, "sysid") == 0){
                dead = 1;
            }
        }
        if(dead == 1){
            fprintf(fp, "vn: %d ", getDocVersion(d));
            for(j = 0; j < getArraySize(f); j++){
                charred = getIndex(f, j);
                if(strcmp(charred, "sysid") == 0)
                    fprintf(fp,"sysid: %d ", getSysID(d));
            }
            for(j = 0; j < getArraySize(f); j++){
                charred = getIndex(f, j);
                if(strcmp(charred, "DocID") == 0){
                    fprintf(fp,"DocID: %d ", getDocID(d));
                }
                else if(hasF(d, charred))
                    fprintf(fp,"%s: %d ", charred, getFieldValue(d, charred));
            }
        }
        if(dead)
            fprintf(fp,"\n");
        dead = 0;
    }
    fprintf(fp,"\n");
    return;
}

void displayResults(Array *arr, FILE*fp)
{
    int i;
    for(i = 0; i < getArraySize(arr); i++)
    {
        Document *d = getIndex(arr, i);
        showDocument(d, fp);
    }
    fprintf(fp,"\n");
    return;
}

void showCount(Array *arr, char *f, FILE *fp)
{
    fprintf(fp,"count_%s: %d\n\n",f, getArraySize(arr));
    return; 
}

void insert(Database *d, Assemblage *c, char *l, int stopped)
{
    Document *doc = createDoc(l, getDBSysID(d), stopped);
    incSId(d);
    addDoc(c, doc);
    return;
}

void sort(Assemblage *c, char *l, int s, FILE *fp)
{
    Array *field = braceValues(l, s);
    char *f = getIndex(field, 0);
    char *stopped = getIndex(field, 1);
    Array *arr = fieldFilter(c, f);
    //showResults(arr);
    arr = filterVersions(arr, c, l, atoi(stopped));
    //showResults(arr);
    sortByField(arr, f, 0, getArraySize(arr) - 1);
    displayResults(arr, fp);
    return;
}

void count(Assemblage *c, char *l, int s, FILE*fp)
{
    Array *field = braceValues(l, s);
    char *f = getIndex(field, 0);
    char *stopped = getIndex(field, 1);
    Array *arr = fieldFilter(c, f);
    arr = filterVersions(arr, c, l, atoi(stopped));
    showCount(arr, f, fp);
    return;
}

void query(Assemblage *c, char *l, int x, FILE*fp)
{
    Array *cond = braceValues(l, x);
    char *conditions = getIndex(cond, 0);
    char *dead = getIndex(cond, 1);
    Array *arr = condtitionFilter(c, conditions);
    
    x = skipComma(l, atoi(dead));
    Array *f = braceValues(l, x);
    char *fields = getIndex(f, 0);
    dead = getIndex(f, 1);
    
    arr = filterVersions(arr, c, l, atoi(dead));
    sortByVersion(arr, getArraySize(arr));
    
    if(strlen(fields) == 0)
        displayResults(arr, fp);
    else
        displayFilteredResults(arr, getFieldsArr(fields), fp);
    return;
}

void determineQuery(Database *d, char *line, int s, char *c, char *q, FILE *f) {
    long size = strlen(line);
    Assemblage *car = getAssemblage(d, c);
    if(line[size-1] == ')')
        line[size-1] = '\0';
    else{
        return;
    }
    if(strcmp(q, "insert") == 0){
        insert(d, car, line, s);
    }
    else if(strcmp(q, "sort") == 0){
        sort(car, line, s, f);
    }
    else if(strcmp(q, "count") == 0){
        count(car, line, s, f);
    }
    else if(strcmp(q, "query") == 0){
        query(car, line, s, f);
    }
    else printf("Error. %s not a valid operation\n", q);
    return;
}

int spaceRipper(char *l)
{
    int i;
    for(i = 0; i < strlen(l); i++) {
        if(l[i] != ' ') break;
    }
    return i;
}

void doQ(Database *d, char *line, FILE *f)
{
    char car[777];
    char q[777];
    int ocar = 1;
    int broke = 0;
    char charred;
    int i;
    int x = 0;
    int start = spaceRipper(line);

    for(i = start; i < strlen(line); i++){
        charred = line[i];
        if(charred == '.') {
            ocar = 0;
            car[x] = '\0';
            x = 0;
        }
        else if(ocar){
            car[x] = charred;
            ++x;
        }
        else if(charred == '('){
            q[x] = '\0';
            broke = i + 1;
            break;
        }
        else{
            q[x] = charred;
            ++x;
        }
    }
    determineQuery(d, line, broke, strdup(car), strdup(q), f);
    return;
}
