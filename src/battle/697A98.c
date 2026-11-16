#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAfterimageInit(void) {
    FxAfterimage *p = &fxAfterimage;
    BtlUnit *e;
    s32 i;
    s32 j;
    u32 max;

    p->unit = 0;
    p->poses = 0;
    for (j = 0; j < 8; j++) {
        p->drawLists[j] = 0;
    }
    fxAfterimageSnap.motion = 0;
    max = 0;
    e = btlUnits;
    for (i = 0; i < 16; i++) {
        if (e->def != NULL && e->def->modelPack != NULL) {
            if (max < e->def->modelPack->gaObjCount) {
                max = e->def->modelPack->gaObjCount;
            }
        }
        e++;
    }
    fxAfterimageSnapDrawList = MemAlloc32(GphDrawListSize(max));
    GphDrawListInit(max, fxAfterimageSnapDrawList);
    GphDrawListSet(fxAfterimageSnapDrawList);
    GphDrawListClear(0);
    fxClone.unit = 0;
}
