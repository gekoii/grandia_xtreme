#include "common.h"
#include "field/event.h"

void OccsWake(s16 i) {
    OccsEntry *p;
    OccsEntry *q;

    if (i < occsTable->count) {
        p = (OccsEntry *)(i * 0x98 + (u32)occsTable->items);
        if (p->state == OCC_SLEEP) {
            p->state = OCC_WAKE;
            p->flags |= OCC_ACTIVE;
            q = (OccsEntry *)(i * 0x98 + (u32)occsTable->items);
            q->timer = 0;
            q->idx = 0;
        }
    }
}
