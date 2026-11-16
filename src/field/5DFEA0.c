#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void EffUpdate(void) {
    s32 i;
    u8 *tbl = (u8 *)camField;

    for (i = 0; i < effMapWork->ringCount; i++) {
        EffObjUpdate(effMapWork->ringList[i], tbl);
    }
    for (i = 0; i < effMapWork->moteCount; i++) {
        EffObjUpdate(effMapWork->moteList[i], tbl);
    }
    for (i = 0; i < effMapWork->fireCount; i++) {
        EffObjUpdate(effMapWork->fireList[i], tbl);
    }
    for (i = 0; i < effMapWork->driftCount; i++) {
        EffObjUpdate(effMapWork->fireList[i], tbl);
    }
    for (i = 0; i < effMapWork->steamCount; i++) {
        EffObjUpdate(effMapWork->steamList[i], tbl);
    }
    for (i = 0; i < 16; i++) {
        EffObjUpdate(effMapWork->pool16[i], tbl);
    }
    for (i = 0; i < 4; i++) {
        EffObjUpdate(effMapWork->poolE0[i], tbl);
        EffObjUpdate(effMapWork->pickupPool[i], tbl);
        EffObjUpdate(effMapWork->pool5A[i], tbl);
        EffObjUpdate(effMapWork->spotOpenPool[i], tbl);
        EffObjUpdate(effMapWork->pool140[i], tbl);
        EffObjUpdate(effMapWork->spotPool[i], tbl);
        EffObjUpdate(effMapWork->charaPool[i], tbl);
    }
}
