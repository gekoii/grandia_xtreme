#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "field/n12.h"
#include "field/n17.h"
#include "protos/field.h"

void FldSkyBuildDrawList(void) {
    s32 i;
    GphModel *o;

    GphDrawListSet(fldSkyDrawList);
    GphDrawListClear(0);
    for (i = 0; i < 2; i++) {
        if (fldSkyTable[i].enable != 0) {
            o = fldSkyTable[i].model;
            if (o != NULL) {
                switch (fldSkyTable[i].follow) {
                case FLD_SKY_FOLLOW_XYZ:
                    o->pos[0] = camField[0].eye[0];
                    o->pos[1] = camField[0].eye[1];
                    o->pos[2] = camField[0].eye[2];
                    break;
                case FLD_SKY_FOLLOW_XZ:
                    o->pos[0] = camField[0].eye[0];
                    o->pos[2] = camField[0].eye[2];
                    break;
                }
                GphModelCalcAndDraw(fldSkyTable[i].model);
            }
        }
    }
    GphDrawListSort();
}
