#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

void GphModelUpdateGsRegs(struct GphModel *);

void MapModelCalcPass(void) {
    s32 i;
    MapModel *e;

    for (i = 0; i < mapModelCount; i++) {
        if (mapModelTable[i].visible) {
            GphModelUpdateGsRegs(mapModelTable[i].model);
        }
    }
    for (i = 0; i < mapModelCount; i++) {
        e = (MapModel *)(i * sizeof(MapModel) + (u32)mapModelTable);
        if (e->visible && e->waveOn) {
            GphModelDeformWave(e->model, e->wavePhase, e->waveAmp, e->waveSpeed);
        }
    }
    FldSpotModelCalcPass();
}
