typedef struct node Node;

Node *newNode(void);
void *getNodeData(Node *);
void setNodeData(Node *, void *);
Node *getNodeNext(Node *);
void setNodeNext(Node *, Node *);
Node *getNodePrevious(Node *);
void setNodePrevious(Node *, Node *);
void freeNode(Node *);
