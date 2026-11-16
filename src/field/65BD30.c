#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetRedGlow(PtclEmitter *arg0, void *pos) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 1;
    arg0->motionMode = 1;
    arg0->dirMode = 1;
    arg0->count = 0x14;
    arg0->unk12B2 = 0;
    arg0->life = 100.0f;
    arg0->sizeRandMode = 0;
    arg0->pointCount = 1;
    arg0->alphaMode = 1;
    arg0->spawnRadius = 300.0f;
    arg0->interval = 30.0f;
    arg0->dir[0] = 0.0f;
    arg0->dir[1] = 0.0f;
    arg0->dir[2] = 0.0f;
    arg0->dir[3] = 0.0f;
    arg0->spread[0] = 0.0f;
    arg0->spread[1] = 0.0f;
    arg0->spread[2] = 0.0f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = 0.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[0] = 300.0f;
    arg0->sizeStart[1] = 300.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 400.0f;
    arg0->sizeEnd[1] = 400.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->sizeRandX = 50.0f;
    arg0->sizeRandY = 50.0f;
    arg0->unk878 = 0.0f;
    arg0->unk87C = 0.0f;
    arg0->points = 0;
    arg0->colorStart[0] = 255.0f;
    arg0->colorStart[1] = 100.0f;
    arg0->colorStart[2] = 100.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[0] = 255.0f;
    arg0->colorEnd[1] = 100.0f;
    arg0->colorEnd[2] = 100.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}
