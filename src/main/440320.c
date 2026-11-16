#include "common.h"
#include "protos/main.h"
#include "main/n06.h"


s32 MemHeapCreateAt(MemBlock *b, s32 size) {
    MemHeap *h;
    MemBlock *b2;

    h = MemHeapEntryAlloc();
    if (h == NULL) {
        return 0;
    }
    b2 = b + 1;
    h->id = memNextHeapId++;
    h->end = (u32)b + size;
    h->base = (u32)b;
    h->size = size;
    h->dirty = 0;
    b->next = b2;
    b->size = 0;
    h->head = b;
    b2->size = size - 0x40;
    h->inUse = 1;
    b2->next = b;
    return h->id;
}
