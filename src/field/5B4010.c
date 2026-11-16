#include "common.h"
#include "field/event.h"

extern void *FldMemAlloc(s32 size);

void OccAlloc(s32 n) {
    void *p;

    occTable->count = n;
    if (n != 0) {
        p = FldMemAlloc(n << 7);
        if (p != NULL) {
            occTable->items = p;
        } else {
            occTable->count = 0;
            occTable->items = NULL;
        }
    }
}
