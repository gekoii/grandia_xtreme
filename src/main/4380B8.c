#include "common.h"
#include "gph.h"

void ChrModelAssignDeformBufs(GphModel *arg0, u32 arg1) {
    s32 n = arg0->gaObjCount;
    GphGaObject *e = arg0->gaObjs;

    arg1 = (arg1 + 0x3F) & 0xFFFFFFC0;
    do {
        n--;
        e->deformBuf0 = (void *)arg1;
        arg1 += (e->vtxCount * 16 + 0x3F) & 0xFFFFFFC0;
        e->deformBuf1 = (void *)arg1;
        arg1 += (e->vtxCount * 8 + 0x4F) & 0xFFFFFFC0;
        e++;
    } while (n != 0);
}
