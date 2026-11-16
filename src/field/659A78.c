#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetAppear(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->motionMode = 5;
    p->spawnFlags = 0;
    p->dirAxis[1] = 1;
    p->dirAxis[0] = 0;
    p->dirAxis[2] = 0;
    p->sizeRandMode = 1;
    p->dirMode = 2;
    p->count = 0x10;
    p->life = 60.0f;
    p->pointCount = 1;
    p->unk12B2 = 0;
    p->interval = 0.1f;
    p->spawnRadius = 100.0f;
    VuVecCopy(p->origin, pos);
    p->dir[0] = 2.4f;
    p->dir[1] = 4.8f;
    p->dir[2] = 2.4f;
    p->dir[3] = 0.0f;
    p->spread[0] = 0.6f;
    p->spread[1] = 3.2f;
    p->spread[2] = 0.6f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 20.0f;
    p->sizeStart[1] = 800.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 0.0f;
    p->sizeEnd[1] = 0.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandX = 80.0f;
    p->sizeRandY = 80.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->points = p->unk12CC;
    p->colorStart[0] = 120.0f;
    p->colorStart[1] = 120.0f;
    p->colorStart[2] = 240.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 200.0f;
    p->colorEnd[1] = 120.0f;
    p->colorEnd[2] = 120.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
