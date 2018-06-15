#include <stdio.h>
#include <stdlib.h>
extern int readInt(FILE *);
extern double readReal(FILE *);
extern char readChar(FILE *);
extern char readRawChar(FILE *);
extern char *readString(FILE *);
extern char *readToken(FILE *);
extern char *readLine(FILE *);
extern void *allocate(size_t);
extern void *reallocate(void *,size_t);

