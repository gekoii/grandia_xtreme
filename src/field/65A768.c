#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetFlameTiny(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 1;
    p->dirMode = 1;
    p->count = 0x1F;
    p->unk12B2 = -1;
    p->motionMode = 0;
    p->life = 15.0f;
    p->interval = 1.0f;
    p->pointCount = 1;
    p->spawnRadius = 5.0f;
    p->sizeRandMode = 0;
    p->dir[0] = 0.05f;
    p->dir[1] = 1.0f;
    p->dir[2] = 0.05f;
    p->dir[3] = 0.0f;
    p->spread[0] = 12.2f;
    p->spread[1] = 1.6f;
    p->spread[2] = 12.2f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 5.0f;
    p->sizeStart[1] = 5.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 1.0f;
    p->sizeEnd[1] = 2.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandX = 6.0f;
    p->sizeRandY = 6.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->colorStart[0] = 120.0f;
    p->colorStart[1] = 64.0f;
    p->colorStart[2] = 32.0f;
    p->colorStart[3] = 128.0f;
    p->points = 0;
    p->colorEnd[0] = 120.0f;
    p->colorEnd[1] = 64.0f;
    p->colorEnd[2] = 32.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
