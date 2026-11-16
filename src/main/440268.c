#include "common.h"
#include "protos/main.h"
#include "main/n06.h"


s32 MemHeapCreate(s32 arg0) {
    MemHeap *h;
    MemBlock *b;
    MemBlock *b2;
    u32 end;

    h = MemHeapEntryAlloc();
    b = (MemBlock *)memArenaCur;
    end = ((u32)b + arg0 + 0x7F) & 0xFFFFFF80;
    arg0 = (arg0 + 0x7F) & 0xFFFFFF80;
    if (memArenaEnd < end) {
        return 0;
    }
    b2 = b + 1;
    h->id = memNextHeapId++;
    h->base = (u32)b;
    h->end = end;
    h->size = arg0;
    memArenaCur = end;
    h->dirty = 0;
    b->next = b2;
    b->size = 0;
    h->head = b;
    b2->size = arg0 - 0x40;
    h->inUse = 1;
    b2->next = b;
    return h->id;
}
