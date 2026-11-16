#include "common.h"
#include "gph.h"




void GphTexUnregister(s32 arg0) {
    u32 i;
    GphTexSlot *e;

    e = gphTexTable->slots;
    for (i = 0; i < gphTexTable->capacity; i++, e++) {
        if ((s32)e->src == arg0) {
            e->flag = 0xFF;
            e->src = 0;
            gphTexTable->count--;
            return;
        }
    }
}
