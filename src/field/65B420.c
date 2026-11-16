#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetItemSparkle(PtclEmitter *arg0, void *arg1) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 0;
    arg0->motionMode = 4;
    arg0->dirMode = 2;
    arg0->dirAxis[1] = 1;
    arg0->dirAxis[0] = 0;
    arg0->count = 0x10;
    arg0->dirAxis[2] = 0;
    arg0->life = 50.0f;
    arg0->sizeRandMode = 1;
    arg0->unk12B2 = 0;
    arg0->pointCount = 1;
    arg0->interval = 0.1f;
    arg0->spawnRadius = 50.0f;
    VuVecCopy(arg0->origin, arg1);
    arg0->dir[0] = 0.8f;
    arg0->sizeStart[0] = 100.0f;
    arg0->dir[3] = 0.0f;
    arg0->dir[2] = 0.8f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->sizeRandX = 40.0f;
    arg0->sizeStart[1] = 100.0f;
    arg0->accel[1] = 0.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->sizeRandY = 40.0f;
    arg0->dir[1] = 3.2f;
    arg0->unk860[0] = 0.0f;
    arg0->spread[0] = 0.6f;
    arg0->unk860[1] = 0.0f;
    arg0->points = arg0->unk12CC;
    arg0->unk860[2] = 0.0f;
    arg0->colorStart[1] = 160.0f;
    arg0->spread[1] = 0.6f;
    arg0->unk860[3] = 0.0f;
    arg0->spread[2] = 0.6f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 0.0f;
    arg0->colorStart[0] = 200.0f;
    arg0->sizeEnd[1] = 0.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->unk878 = 0.0f;
    arg0->colorStart[2] = 80.0f;
    arg0->unk87C = 0.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[0] = 60.0f;
    arg0->colorEnd[1] = 30.0f;
    arg0->colorEnd[2] = 200.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}

