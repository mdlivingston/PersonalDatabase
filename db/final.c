#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "document.h"
#include "assemblage.h"
#include "scanner.h"
#include "doublylinked.h"

void processDFile(FILE *fp, Assemblage *c, Database *db)
{
    char *line = readLine(fp);
    while(!feof(fp))
    {
        Document *d = createDoc(line, getDBSysID(db), 0);
        incSId(db);
        addDoc(c, d);
        free(line);
        line = readLine(fp);
    }
    free(line);
    return;
}

void processQFile(FILE *fp, Database *db)
{
     FILE *f = fopen("mdlivingston.txt", "w");
     if (f == NULL)
     {
     printf("Error opening file!\n");
     exit(1);
     }
    
    char *line = readLine(fp);
    while(!feof(fp))
    {
        fprintf(f, "Query: %s\n", line);
        fprintf(f, "\n");
        doQ(db, line, f);
        free(line);
        line = readLine(fp);
    }
    free(line);
    fclose(f);
    return;
}

int main(void)
{
    Database *db = newDatabase("db");
    FILE *data = fopen("data.txt", "r");
    FILE *queries = fopen("queries.txt", "r");
    Assemblage *c = newAssemblage("final", data);
    addAssemblage(db, c);
    processDFile(data, c, db);
    processQFile(queries, db);
    fclose(data);
    fclose(queries);
    return 0;
}
