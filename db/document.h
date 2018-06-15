typedef struct document Document;

Document *createDoc(char *, int, int);
int getDocVersion(Document *);
int getDocID(Document *);
int getSysID(Document *);
void setDocVersion(Document *, int);
void setDocLatest(Document *, int);
void showDocument(Document *, FILE*);
int hasF(Document *, char *);
int getFieldValue(Document *, char *);
int isLatest(Document *);


