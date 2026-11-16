#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetSparkleRise(PtclEmitter *arg0, void *arg1) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 0;
    arg0->unk12B2 = 0;
    arg0->motionMode = 1;
    arg0->dirMode = 1;
    arg0->count = 0x1F;
    arg0->life = 60.0f;
    arg0->sizeRandMode = 2;
    arg0->pointCount = 1;
    arg0->interval = 1.0f;
    arg0->spawnRadius = 80.0f;
    VuVecCopy(arg0->origin, arg1);
    arg0->dir[0] = 0.5f;
    arg0->spread[0] = 1.2f;
    arg0->sizeStart[0] = 2.0f;
    arg0->sizeEnd[0] = 10.0f;
    arg0->sizeRandX = 20.0f;
    arg0->colorStart[0] = 128.0f;
    arg0->colorStart[1] = 128.0f;
    arg0->colorStart[2] = 128.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->dir[3] = 0.0f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = 0.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->unk878 = 0.0f;
    arg0->unk87C = 0.0f;
    arg0->dir[1] = 8.0f;
    arg0->dir[2] = 0.5f;
    arg0->spread[1] = 0.6f;
    arg0->spread[2] = 1.2f;
    arg0->sizeStart[1] = 2.0f;
    arg0->sizeEnd[1] = 10.0f;
    arg0->sizeRandY = 20.0f;
    arg0->colorEnd[0] = 148.0f;
    arg0->colorEnd[1] = 128.0f;
    arg0->points = arg0->unk12CC;
    arg0->colorEnd[2] = 128.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}
