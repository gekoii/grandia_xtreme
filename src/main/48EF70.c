/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct Node Node;
typedef struct List List;

struct List {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    Node *head;
    s32 tail;
};

struct Node {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0xC];
    s32 unk1C;
    void *unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[4];
    Node *next;
    List *list;
};

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    void *unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    Node *node;
} Src;

void iWakeupThread(s32, Node *, List *);

void _request_call(Src *arg0) {
    Node *n;
    List *l;

    n = arg0->node;
    l = n->list;
    if (l->head == NULL) {
        l->head = n;
    } else {
        ((Node *)l->tail)->next = n;
    }
    l->tail = (s32)n;
    n->unk20 = arg0->unk14;
    n->unk1C = arg0->unk1C;
    n->unk24 = arg0->unk20;
    n->unkC = arg0->unk24;
    n->unk28 = arg0->unk28;
    n->unk2C = arg0->unk2C;
    n->unk30 = arg0->unk30;
    n->unk34 = arg0->unk10;
    if (l->unk0 >= 0 && l->unk4 == 0) {
        iWakeupThread(l->unk0, n, l);
    }
}
