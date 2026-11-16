#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void VuVecCopy(void *, void *);

void FldTreasureSetPosById(u8 id, void *mtx) {
    s32 i;
    FldTreasure *e;

    for (i = 0; i < 64; i++) {
        e = &fldTreasureWork[i];
        if (e->active && e->id == id) {
            VuVecCopy(ChrInstGetPos(fldTreasureWork[i].instGroup, fldTreasureWork[i].inst), mtx);
            VuVecCopy(fldTreasureWork[i].path[0], mtx);
            VuVecCopy(fldTreasureWork[i].path[1], mtx);
            VuVecCopy(fldTreasureWork[i].path[2], mtx);
            VuVecCopy(fldTreasureWork[i].path[3], mtx);
            return;
        }
    }
}
