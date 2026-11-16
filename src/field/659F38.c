#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetPurpleHaze(PtclEmitter *p, void *pos) {
    p->motionMode = 1;
    p->spawnFlags = 0;
    p->dirAxis[0] = 0;
    p->dirAxis[2] = 0;
    p->alphaMode = 0;
    p->unk12B2 = 0;
    p->dirAxis[1] = 1;
    p->spawnRadius = 0.0f;
    p->sizeRandMode = 1;
    p->dirMode = 2;
    p->count = 0x1F;
    p->life = 500.0f;
    p->pointCount = 1;
    p->interval = 1.5f;
    VuVecCopy(p->origin, pos);
    p->dir[0] = 10.12f;
    p->dir[1] = 22.0f;
    p->dir[2] = 10.12f;
    p->dir[3] = 0.0f;
    p->spread[0] = 15.2f;
    p->spread[1] = 10.001f;
    p->spread[2] = 15.2f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 100.0f;
    p->sizeStart[1] = 100.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 300.0f;
    p->sizeEnd[1] = 300.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandX = 5.0f;
    p->sizeRandY = 0.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->points = p->unk12CC;
    p->colorStart[0] = 160.0f;
    p->colorStart[1] = 64.0f;
    p->colorStart[2] = 144.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 160.0f;
    p->colorEnd[1] = 64.0f;
    p->colorEnd[2] = 144.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
