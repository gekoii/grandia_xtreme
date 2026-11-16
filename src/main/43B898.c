#include "common.h"
#include "protos/main.h"
#include "gph.h"

extern void VuMtxMul(u32, s32, u32);

void ChrModelPartMulMtxById(GphModel *arg0, s32 arg1, s32 arg2) {
    GphPart *e;
    u32 s;
    u32 end;

    if (arg0 == NULL) {
        return;
    }
    e = ChrModelFindPart(arg0, arg1);
    if (e == NULL) {
        return;
    }
    s = (u32)e->mtx;
    if (e->next != 0) {
        end = (u32)e[e->next].mtx;
    } else {
        { s32 off = arg0->partCount * sizeof(GphPart); end = (u32)((GphPart *)((u8 *)arg0->parts + off))[-1].mtx + 0x40; }
    }
    do {
        VuMtxMul(s, arg2, s);
        s += 0x40;
    } while (s < end);
}
