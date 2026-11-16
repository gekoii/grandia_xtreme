#include "common.h"
#include "gph.h"

void GphTexReplace(GphTex *arg0, u8 arg1) {
    GphTexSlot *e = gphTexTable->slots;
    GphTexSlot *p = (GphTexSlot *)((u32)(arg1 * 0x2C0) + (u32)e);

    if (p->flag != 0xFF) {
        p->src = arg0;
        p->flag = arg1;
    }
}
