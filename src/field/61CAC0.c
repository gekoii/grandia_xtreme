#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void GphModelUpdateGsRegs(s32);

void FldSpotModelCalcPass(void) {
    FldSpot *e;
    s32 n;
    u32 i;

    e = (FldSpot *)FldSpotGetList();
    n = FldSpotGetCount();
    if (e == NULL) {
        return;
    }
    for (i = 0; (s32)i < n; i++) {
        if (e->unkA != 0) {
            GphModelUpdateGsRegs((s32)e->model);
        }
    }
    for (i = 0; i < fldSpotWork->modelCount; i++) {
        GphModelUpdateGsRegs((s32)fldSpotWork->models[i]);
    }
}
