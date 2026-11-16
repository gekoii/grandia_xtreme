#include "common.h"
#include "gph.h"
#include "chr.h"
#include "battle/n31.h"

u8 BtlChrTexFindSlotById(GphTexTable *slots, ChrModelData *g, s32 id) {
    GphGaObject *e = g->gaObjs;
    u32 i;

    if (id == 0xFF) {
        return 0;
    }
    for (i = 0; i < g->gaObjCount; i++, e++) {
        if (((BtlTexTableRow *)slots)[e->unk3].src->texId == id) {
            return e->unk3;
        }
    }
    return 0;
}
