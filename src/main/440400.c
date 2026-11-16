#include "common.h"
#include "main/n06.h"

MemHeap *MemHeapEntryAlloc(void) {
    s32 i;
    MemHeap *e = memHeapTable->heaps;

    for (i = 0; i < memHeapTable->count; i++, e++) {
        if (e->inUse == 0) {
            e->inUse = 1;
            return e;
        }
    }
    return NULL;
}
