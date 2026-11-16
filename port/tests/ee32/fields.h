typedef struct Node { struct Node *next; int v; char *names[3]; void (*fn)(int); } Node;
typedef struct Pair { Node *a, *b; int n; } Pair;
