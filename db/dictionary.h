typedef struct dictionary Dictionary;
typedef struct keyvalue KeyVal;

Dictionary *newDictionary(void);
void addKVF(Dictionary *, char *, void *);
void displayDictionary(Dictionary *, FILE*);
int containsKey(Dictionary *, char *);
void *getValue(Dictionary *, char *);

