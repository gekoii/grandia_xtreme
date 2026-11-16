/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n17.h"


void MapModelUpdate(void) {
    ChrObj b;
    s32 i;
    MapModel *e;

    for (i = 0; i < mapModelCount; i++) {
        if (mapModelTable[i].visible) {
            b.model = mapModelTable[i].model;
            b.motTable = &mapModelTable[i].motTable;
            ChrMotBlendUpdate(&b);
            ChrMotUpdate(mapModelTable[i].model, 1.0f);
        }
    }
    FldSpotUpdateMotion();
    for (i = 0; i < mapModelCount; i++) {
        e = (MapModel *)(i * sizeof(MapModel) + (u32)mapModelTable);
        if (e->visible && e->waveOn) {
            e->wavePhase += 1.0f;
            if (e->wavePhase >= 360.0f) {
                e->wavePhase -= 360.0f;
            }
        }
    }
}
