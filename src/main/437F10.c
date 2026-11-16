#include "common.h"
#include "gph.h"

void ChrModelAssignVtxBufs(GphModel *arg0, s32 arg1, s32 arg2) {
    GphGaObject *e;
    s32 n;
    s32 addr;

    if (arg0 != NULL) {
        n = arg0->gaObjCount;
        e = arg0->gaObjs;
        addr = (arg1 + 0x3F) & 0xFFFFFFC0;
        do {
            if (arg2 & 1) {
                e->vtxPkt = (u8 *)addr;
                addr += ((e->vtxCount * 16) + 0x3F) & 0xFFFFFFC0;
            }
            if (arg2 & 2) {
                e->deformBuf0 = (void *)addr;
                addr += ((e->vtxCount * 8) + 0x4F) & 0xFFFFFFC0;
            }
            if (arg2 & 4) {
                e->deformBuf1 = (void *)addr;
                addr += ((e->vtxCount * 4) + 0x4F) & 0xFFFFFFC0;
            }
            n--;
            if (arg2 & 8) {
                e->envUv = (void *)addr;
                addr += ((e->vtxCount * 4) + 0x4F) & 0xFFFFFFC0;
            }
            e++;
        } while (n != 0);
    }
}
