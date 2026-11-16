#include "common.h"
#include "gph.h"
#include "chr.h"
#include "battle/n31.h"

void BtlChrTexRemapToSlots(ChrModelData *g, GphTexTable *slots, ChrModelData *g2) {
    u32 i = 0;
    GphGaObject *e = g->gaObjs;

    for (; i < g->gaObjCount; i++, e++) {
        e->unk3 = BtlChrTexFindSlotById(slots, g2, e->unk3);
    }
}
