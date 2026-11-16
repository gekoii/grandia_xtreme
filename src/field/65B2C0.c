#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetSmokeGrey(PtclEmitter *arg0, void *pos) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 0;
    arg0->motionMode = 1;
    arg0->dirMode = 2;
    arg0->dirAxis[0] = 0;
    arg0->dirAxis[1] = 1;
    arg0->dirAxis[2] = 0;
    arg0->count = 0x14;
    arg0->alphaMode = 2;
    arg0->fadeFrames = 0x19;
    arg0->unk12B2 = 0;
    arg0->life = 45.0f;
    arg0->sizeRandMode = 0;
    arg0->pointCount = 1;
    arg0->spawnRadius = 0.0f;
    arg0->interval = 0.25f;
    arg0->points = 0;
    arg0->origin[0] = 0.0f;
    arg0->origin[1] = 0.0f;
    arg0->origin[2] = 0.0f;
    arg0->origin[3] = 0.0f;
    arg0->dir[0] = 2.2f;
    arg0->dir[1] = 0.5f;
    arg0->dir[2] = 2.2f;
    arg0->dir[3] = 0.0f;
    arg0->spread[0] = 1.6f;
    arg0->spread[1] = 1.0f;
    arg0->spread[2] = 1.6f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = 0.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[0] = 28.0f;
    arg0->sizeStart[1] = 28.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 96.0f;
    arg0->sizeEnd[1] = 96.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->colorStart[0] = 64.0f;
    arg0->colorStart[1] = 64.0f;
    arg0->colorStart[2] = 64.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[0] = 64.0f;
    arg0->colorEnd[1] = 64.0f;
    arg0->colorEnd[2] = 64.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}
