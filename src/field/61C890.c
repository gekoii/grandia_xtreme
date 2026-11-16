#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void FldSpotRender(void) {
    FldSpot *e;
    s32 n;
    s32 i;

    e = (FldSpot *)FldSpotGetList();
    n = FldSpotGetCount();
    if (e == NULL) {
        return;
    }
    for (i = 0; i < n; i++, e++) {
        if (e->unkA != 0) {
            GphModelCalcAndDraw(e->model);
        }
    }
    for (i = 0; i < fldSpotWork->modelCount; i++) {
        GphModelCalcAndDraw(fldSpotWork->models[i]);
    }
}
