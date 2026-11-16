#include "common.h"
#include "gph.h"
#include "chr.h"
#include "battle/n31.h"

GphTex *BtlChrTexFindById(GphTexTable *slots, ChrModelData *g, s32 id) {
    GphGaObject *e = g->gaObjs;
    u32 i;
    GphTex *o;

    if (id == 0xFF) {
        return NULL;
    }
    for (i = 0; i < g->gaObjCount; i++, e++) {
        o = ((BtlTexTableRow *)slots)[e->unk3].src;
        if (o->texId == id) {
            return o;
        }
    }
    return NULL;
}
