#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"
#include "field/n12.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

extern void VuMtxMul(void *, void *, void *);

void AmapBuildDrawList(void) {
    u32 i;

    if (amapState->loaded != 0) {
        for (i = 0; i < amapState->cellInstCount; i++) {
            VuMtxMul(amapCellInsts[i].viewMtx, camField, amapCellInsts[i].mtx);
        }
        i = 0;
        GphDrawListSet(fldMapDrawListSize[1]);
        if (amapState->objModelCount != 0) {
            do {
                if (amapObjModels[i] != NULL) {
                    GphModelCalcAndDraw(amapObjModels[i]);
                }
                i++;
            } while (i < amapState->objModelCount);
        }
        GphDrawListSort();
        GphDrawListEnvMapCalcUv();
        copyq(amapState->unk10, (void *)0x70002740);
    }
}
