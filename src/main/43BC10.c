#include "common.h"
#include "gph.h"
#include "protos/main.h"

#define UNIT_MATRIX(m) \
    __asm__ volatile( \
        "vmr32.xyzw $vf5, $vf0\n\t" \
        "vmr32.xyzw $vf6, $vf5\n\t" \
        "vmr32.xyzw $vf7, $vf6\n\t" \
        "sqc2 $vf0, 0x30(%0)\n\t" \
        "sqc2 $vf5, 0x20(%0)\n\t" \
        "sqc2 $vf6, 0x10(%0)\n\t" \
        "sqc2 $vf7, 0x0(%0)" \
        : : "r"(m) : "memory")

void MemMove128(void *, void *, s32);
void GphGaObjEnvMapCalcUv(GphGaObject *, s32); /* 0x0043BD80, still asm */

void GphDrawListEnvMapCalcUv(void) {
    FMATRIX m;
    GphDrawList *mgr = gphRenderCtx->drawList;
    u16 *list = mgr->envMapList;
    u16 *p;
    GphDrawItem *base;
    GphDrawItem *e;
    s32 n;

    if (*list != 0) {
        p = list + 1;
        base = mgr->items;
        UNIT_MATRIX(m);
        n = *list;
        do {
            e = &base[*p];
            n--;
            p++;
            e->gaObj->envUv = (void *)GsPacketAlloc64(e->gaObj->vtxCount * 8 + 0x40);
            MemMove128(e->gaObj->envUv, e->gaObj->uvs, 1);
            GphGaObjEnvMapCalcUv(e->gaObj, e->mtx);
        } while (n != 0);
    }
}
