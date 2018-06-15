#include "document.h"
#include "array.h"

typedef struct Assemblage Assemblage;

Assemblage *newAssemblage(char *, FILE *);
char *getAssemblageName(Assemblage *);
void addDoc(Assemblage *, Document *);
void showDocs(Assemblage *, FILE*);
Array *fieldFilter(Assemblage *, char *);
Array *latestFilter(Assemblage *, Array *);
Array *versionNumberFilter(Assemblage *, Array *, int);
Array *condtitionFilter(Assemblage *, char *);


