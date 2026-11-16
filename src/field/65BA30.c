#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetSmokeLarge(PtclEmitter *arg0, void *pos) {
    PtclEmitterReset(arg0);
    arg0->spawnFlags = 0;
    arg0->count = 0x10;
    arg0->motionMode = 4;
    arg0->dirMode = 2;
    arg0->dirAxis[0] = 0;
    arg0->dirAxis[1] = 1;
    arg0->dirAxis[2] = 1;
    arg0->alphaMode = 1;
    arg0->sizeRandMode = 0;
    arg0->unk12B2 = 0;
    arg0->life = 120.0f;
    arg0->pointCount = 1;
    arg0->interval = 1.0f;
    arg0->spawnRadius = 200.0f;
    arg0->dir[0] = 2.6f;
    arg0->dir[1] = 4.0f;
    arg0->dir[2] = 2.5f;
    arg0->dir[3] = 0.0f;
    arg0->spread[0] = 1.5f;
    arg0->spread[1] = 1.4f;
    arg0->spread[2] = 1.5f;
    arg0->spread[3] = 0.2f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = 0.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[0] = 96.0f;
    arg0->sizeStart[1] = 96.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 192.0f;
    arg0->sizeEnd[1] = 192.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->sizeRandX = 0.0f;
    arg0->sizeRandY = 0.0f;
    arg0->unk878 = 0.0f;
    arg0->unk87C = 0.0f;
    arg0->points = 0;
    arg0->colorStart[0] = 96.0f;
    arg0->colorStart[1] = 80.0f;
    arg0->colorStart[2] = 48.0f;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[0] = 80.0f;
    arg0->colorEnd[1] = 64.0f;
    arg0->colorEnd[2] = 32.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}
