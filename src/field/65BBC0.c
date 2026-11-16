#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetGoldSpray(PtclEmitter *arg0, void *pos) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 0;
    arg0->motionMode = 1;
    arg0->dirMode = 1;
    arg0->count = 0x1F;
    arg0->unk12B2 = 0;
    arg0->life = 300.0f;
    arg0->sizeRandMode = 2;
    arg0->pointCount = 1;
    arg0->interval = 1.0f;
    arg0->spawnRadius = 100.0f;
    arg0->origin[0] = 0.0f;
    arg0->origin[1] = 0.0f;
    arg0->origin[2] = 0.0f;
    arg0->origin[3] = 0.0f;
    arg0->dir[0] = 0.6f;
    arg0->dir[1] = 60.0f;
    arg0->dir[2] = 0.6f;
    arg0->dir[3] = 0.0f;
    arg0->spread[0] = 15.2f;
    arg0->spread[1] = 0.6f;
    arg0->spread[2] = 15.2f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = -60.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[0] = 4.0f;
    arg0->sizeStart[1] = 4.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 40.0f;
    arg0->sizeEnd[1] = 40.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->sizeRandX = 40.0f;
    arg0->sizeRandY = 40.0f;
    arg0->unk878 = 0.0f;
    arg0->unk87C = 0.0f;
    arg0->points = 0;
    arg0->colorStart[0] = 148.0f;
    arg0->colorStart[1] = 112.0f;
    arg0->colorStart[2] = 8.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[0] = 148.0f;
    arg0->colorEnd[1] = 112.0f;
    arg0->colorEnd[2] = 8.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}
