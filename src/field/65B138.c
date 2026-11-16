#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetDarkSmoke(PtclEmitter *arg0, void *pos) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 0;
    arg0->motionMode = 1;
    arg0->dirMode = 2;
    arg0->dirAxis[0] = 0;
    arg0->dirAxis[1] = 1;
    arg0->dirAxis[2] = 0;
    arg0->count = 0x1F;
    arg0->alphaMode = 1;
    arg0->fadeFrames = 0;
    arg0->unk12B2 = -1;
    arg0->life = 60.0f;
    arg0->sizeRandMode = 0;
    arg0->pointCount = 1;
    arg0->interval = 1.0f;
    arg0->spawnRadius = 100.0f;
    arg0->origin[0] = 0.0f;
    arg0->origin[1] = 0.0f;
    arg0->origin[2] = 0.0f;
    arg0->origin[3] = 0.0f;
    arg0->dir[0] = 4.0f;
    arg0->dir[1] = 3.0f;
    arg0->dir[2] = 4.0f;
    arg0->dir[3] = 0.0f;
    arg0->spread[0] = 2.0f;
    arg0->spread[1] = 0.5f;
    arg0->spread[2] = 2.0f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = -2.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[0] = 40.0f;
    arg0->sizeStart[1] = 40.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 150.0f;
    arg0->sizeEnd[1] = 150.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->sizeRandX = 40.0f;
    arg0->sizeRandY = 40.0f;
    arg0->unk878 = 0.0f;
    arg0->unk87C = 0.0f;
    arg0->points = 0;
    arg0->colorStart[0] = 10.0f;
    arg0->colorStart[1] = 20.0f;
    arg0->colorStart[2] = 30.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[0] = 10.0f;
    arg0->colorEnd[1] = 20.0f;
    arg0->colorEnd[2] = 30.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}
