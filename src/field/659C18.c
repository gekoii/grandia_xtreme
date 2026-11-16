#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetSmoke(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->dirAxis[0] = 0;
    p->alphaMode = 0;
    p->sizeRandMode = 0;
    p->unk12B2 = 0;
    p->count = 0x10;
    p->motionMode = 4;
    p->dirMode = 2;
    p->dirAxis[1] = 1;
    p->life = 60.0f;
    p->dirAxis[2] = 1;
    p->pointCount = 1;
    p->spawnRadius = 100.0f;
    p->interval = 0.2f;
    p->dir[0] = 3.6f;
    p->dir[1] = 12.0f;
    p->dir[2] = 6.5f;
    p->dir[3] = 0.0f;
    p->spread[0] = 1.5f;
    p->spread[1] = 1.4f;
    p->spread[2] = 1.5f;
    p->spread[3] = 0.2f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 64.0f;
    p->sizeStart[1] = 64.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 150.0f;
    p->sizeEnd[1] = 150.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandX = 0.0f;
    p->sizeRandY = 0.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->points = 0;
    p->colorStart[0] = 96.0f;
    p->colorStart[1] = 80.0f;
    p->colorStart[2] = 48.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 80.0f;
    p->colorEnd[1] = 64.0f;
    p->colorEnd[2] = 32.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
