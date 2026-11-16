#include "common.h"
#include "gph.h"

void OccPartTreeSetFlags(GphPart *n, s32 bits, s32 mode) {
    switch (mode) {
    case 0:
        n->flags |= bits;
        break;
    case 1:
        n->flags &= ~bits;
        break;
    case 2:
        n->flags ^= bits;
        break;
    default:
        return;
    }
    if (n->child != 0) {
        OccPartTreeSetFlags((GphPart *)(n->child * 0xA0 + (u32)n), bits, mode);
    }
    if (n->next != 0) {
        OccPartTreeSetFlags((GphPart *)(n->next * 0xA0 + (u32)n), bits, mode);
    }
}
