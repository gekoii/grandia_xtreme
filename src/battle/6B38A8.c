#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "chr.h"
#include "battle/n31.h"

void MemSet(void *, s32, s32);

void BtlChrTexRemap(ChrModelData *g, GphTexTable *slots, ChrModelData *g2) {
    u8 old[8];
    u8 new[8];
    u8 n;
    u32 i;
    s32 k;
    s32 j;
    GphGaObject *e;
    void *o;

    MemSet(old, 0xFF, 8);
    n = 0;
    e = g->gaObjs;
    for (i = 0; i < g->gaObjCount; i++, e++) {
        for (k = 0; k < 8; k++) {
            if (e->unk3 == old[k]) {
                break;
            }
        }
        if (k < 8) {
            continue;
        }
        if (n >= 8) {
            break;
        }
        o = BtlChrTexFindById(slots, g2, e->unk3);
        if (o != NULL) {
            new[n] = GphTexRegister(o);
            old[n] = e->unk3;
            n++;
        }
    }
    e = g->gaObjs;
    for (i = 0; i < g->gaObjCount; i++, e++) {
        if (e->unk3 == 0xFF) {
            e->unk3 = 0;
            continue;
        }
        for (j = 0; j < 8; j++) {
            if (e->unk3 == old[j]) {
                e->unk3 = new[j];
                break;
            }
        }
        if (j >= 8) {
            e->unk3 = 0;
        }
    }
}
