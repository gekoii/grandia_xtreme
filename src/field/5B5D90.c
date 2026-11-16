#include "common.h"
#include "field/event.h"

extern void *FldMemAlloc(s32 size);

void OccsAlloc(s32 n) {
    void *p;

    occsTable->count = n;
    if (n != 0) {
        p = FldMemAlloc(n * 0x98);
        if (p != NULL) {
            occsTable->items = p;
        } else {
            occsTable->count = 0;
            occsTable->items = NULL;
        }
    }
}
