#include "common.h"
#include "main/n06.h"

MemHeap *MemHeapFind(s32 id) {
    s32 i;
    s32 n;
    MemHeap *e;
    i = 0;
    n = memHeapTable->count;
    e = memHeapTable->heaps;
    for (; i < n; i++, e++) {
        if (e->id == id) {
            return e;
        }
    }
    return NULL;
}
