#include "common.h"
#include "protos/main.h"
#include "main/n06.h"

/* The 0x14 bytes before an allocation: MemBlock.self and guard. */
typedef struct { MemBlock *bp; s32 used; u8 pad[0xC]; } Tag;


s32 MemFree(MemBlock *p) {
    Tag *t;
    MemBlock *bp;
    MemHeap *h;
    u32 a;

    a = (u32)p;
    t = (Tag *)p - 1;
    p = NULL;
    bp = t->bp;
    h = memCurHeap;
    if (a < h->base || a > h->end) {
        h = MemHeapFind(bp->heapId);
    }
    p = h->head;
    for (; !(bp > p && bp < p->next); p = p->next) {
        if (p >= p->next && (bp > p || bp < p->next)) {
            break;
        }
    }
    if ((MemBlock *)((u8 *)bp + bp->size) == p->next) {
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }
    if ((MemBlock *)((u8 *)p + p->size) == bp) {
        p->size += bp->size;
        p->next = bp->next;
    } else {
        p->next = bp;
    }
    t->used = 0;
    return 0;
}
