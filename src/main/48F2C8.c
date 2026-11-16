/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct Node {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
    s32 x14;
    u8 pad18[0x20];
    struct Node *next;
    struct Node *x3C;
    struct Owner *owner;
} Node;

typedef struct Owner {
    s32 x0;
    s32 x4;
    struct Node *head;
} Owner;

void DIntr(void);
void EIntr(void);

void sceSifRegisterRpc(Node *n, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, Owner *o) {
    Node *p;

    DIntr();
    n->x3C = 0;
    n->next = NULL;
    n->x0 = a1;
    n->x4 = a2;
    n->x8 = a3;
    n->x10 = a4;
    n->x14 = a5;
    n->owner = o;
    if (o->head == NULL) {
        o->head = n;
    } else {
        for (p = o->head; p->next != NULL; p = p->next) {
        }
        p->next = n;
    }
    EIntr();
}
