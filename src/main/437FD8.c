#include "common.h"
#include "gph.h"

void ChrPartAssignVtxBufs(GphPart *obj, s32 addr, s32 flags) {
    GphGaObject *e;
    s32 n;

    if (obj == NULL) {
        return;
    }
    n = obj->gaObjCount;
    addr = (addr + 0x3F) & 0xFFFFFFC0;
    e = obj->gaObjs;
    while (n != 0) {
        if (flags & 1) {
            e->vtxPkt = (u8 *)addr;
            addr += (e->vtxCount * 16 + 0x3F) & 0xFFFFFFC0;
        }
        if (flags & 2) {
            e->deformBuf0 = (void *)addr;
            addr += (e->vtxCount * 8 + 0x4F) & 0xFFFFFFC0;
        }
        if (flags & 4) {
            e->deformBuf1 = (void *)addr;
            addr += (e->vtxCount * 4 + 0x4F) & 0xFFFFFFC0;
        }
        n--;
        if (flags & 8) {
            e->envUv = (void *)addr;
            addr += (e->vtxCount * 4 + 0x4F) & 0xFFFFFFC0;
        }
        e++;
    }
}
