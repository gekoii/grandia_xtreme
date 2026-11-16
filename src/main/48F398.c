/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct Node {
    u8 pad0[0x38];
    struct Node *next;
} Node;

typedef struct {
    u8 pad0[8];
    Node *head;
} List;

void DIntr(void);
void EIntr(void);

Node *sceSifRemoveRpc(Node *n, List *l) {
    Node *p;

    DIntr();
    p = l->head;
    if (p == n) {
        l->head = p->next;
    } else {
        while (p != NULL) {
            if (p->next == n) {
                p->next = n->next;
                break;
            }
            p = p->next;
        }
    }
    EIntr();
    return p;
}
