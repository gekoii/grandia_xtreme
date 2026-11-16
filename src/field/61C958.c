#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void FldSpotUpdateMotion(void) {
    ChrObj obj;
    FldSpot *e;
    s32 n;
    s32 i;

    e = (FldSpot *)FldSpotGetList();
    if (e == NULL) {
        return;
    }
    n = FldSpotGetCount();
    obj.motTable = &fldSpotWork->motTable;
    for (i = 0; i < n; i++, e++) {
        if (e->unkA != 0) {
            obj.model = e->model;
            ChrMotBlendUpdate(&obj);
            ChrMotUpdate((s32)obj.model, 1.0f);
        }
    }
    for (i = 0; i < fldSpotWork->modelCount; i++) {
        obj.model = fldSpotWork->models[i];
        ChrMotBlendUpdate(&obj);
        ChrMotUpdate((s32)obj.model, 1.0f);
    }
}
