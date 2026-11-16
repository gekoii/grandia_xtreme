#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void MemSet(void *, s32, s32);

void FldTreasureRemove(u32 idx) {
    FldTreasure *e;
    FldTreasure *e2;

    if (idx < 64) {
        e = &fldTreasureWork[idx];
        if (e->state) {
            if (e->flagId >= 0) {
                if (e->flagId >= 0x400) {
                    EvtGameFlagSet(e->flagId - 0x400, 1);
                } else {
                    EvtMapTreasureFlagSet(e->flagId, 1);
                }
            }
            e2 = &fldTreasureWork[idx];
            ChrInstFree(e2->instGroup, e2->inst);
            MemSet(&fldTreasureWork[idx], 0, 0x90);
        }
    }
}
