#include "common.h"
#include "protos/main.h"
#include "gph.h"

void VuMtxMul(u32, s32, u32);

void ChrModelPartMulMtx(GphModel *list, s32 index, s32 arg2) {
    GphPart *e;
    u32 p;
    u32 end;

    if (list == NULL) {
        return;
    }
    e = ChrModelPartAt(list, index);
    if (e == NULL) {
        return;
    }
    p = (u32)e->mtx;
    if (e->next != 0) {
        end = (u32)e[e->next].mtx;
    } else {
        { s32 off = list->partCount * sizeof(GphPart); end = (u32)((GphPart *)((u8 *)list->parts + off))[-1].mtx + 0x40; }
    }
    do {
        VuMtxMul(p, arg2, p);
        p += 0x40;
    } while (p < end);
}
