#include "assemblage.h"

typedef struct database Database;

Database *newDatabase(char *);
int getDBSysID(Database *d);
void incSId(Database *d);
void addAssemblage(Database *, Assemblage *);
Assemblage *getAssemblage(Database *, char *);
void showAssemblages(Database *, FILE*); 
void doQ(Database *, char *, FILE*);

