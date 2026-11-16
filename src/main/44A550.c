#include "common.h"
#include "ptcl.h"

void PtclSystemInit(PtclSystem *p, s32 na, s32 nb, s32 nc) {
    void **a;
    PtclEffector *b;
    PtclStarterSlot *c;
    s32 t;
    u32 mask;
    s32 i;
    s32 j;

    mask = 0xFFFFFFF0;
    t = (s32)p + 0x18;
    t = (t + 0xF) & mask;
    p->gens = (PtclGen **)t;
    t += na * 4;
    t = (t + 0xF) & mask;
    p->genCount = na;
    p->effectorCount = nb;
    p->effectors = (PtclEffector *)t;
    p->slotCount = nc;
    t += nb * 0x1C;
    t = (t + 0xF) & mask;
    p->slots = (PtclStarterSlot *)t;
    for (i = na, a = (void **)p->gens; i > 0; i--) {
        *a++ = NULL;
    }
    for (b = p->effectors; nb > 0; nb--) {
        b->id = -1;
        b++;
    }
    for (j = 0, c = p->slots; j < nc; j++) {
        c->index = j;
        c->ctrl = NULL;
        c++;
    }
    ptclCurSystem = p;
}
