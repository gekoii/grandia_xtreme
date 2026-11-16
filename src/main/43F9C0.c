#include "common.h"
#include "gph.h"

GphTexSlot *GphTexGetSlot(u8 arg0) {
    GphTexSlot *base;
    s32 n = (arg0 != 0) ? arg0 : 1;
    base = gphTexTable->slots;
    return base + n;
}
