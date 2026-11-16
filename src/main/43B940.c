#include "common.h"
#include "gph.h"

void VuMtxMul(u32, s32, u32);

void ChrPartMulMtx(GphModel *m, GphPart *p, s32 arg2) {
    u32 a;
    u32 end;

    if (m == NULL) {
        return;
    }
    if (p == NULL) {
        return;
    }
    a = (u32)p->mtx;
    if (p->next != 0) {
        end = (u32)p[p->next].mtx;
    } else {
        GphPart *q = &m->parts[m->partCount];
        end = (u32)q[-1].mtx + 0x40;
    }
    do {
        VuMtxMul(a, arg2, a);
        a += 0x40;
    } while (a < end);
}
