#include "common.h"
#include "protos/field.h"
#include "field/n12.h"
#include "field/n17.h"


void MapModelBuildDrawList(void) {
    s32 i;

    GphDrawListSet(fldMapDrawList);
    GphDrawListClear(0);
    for (i = 0; i < mapModelCount; i++) {
        if (mapModelTable[i].visible) {
            GphModelCalcAndDraw(mapModelTable[i].model);
        }
    }
    FldSpotRender();
    GphDrawListSort();
    GphDrawListEnvMapCalcUv();
}
