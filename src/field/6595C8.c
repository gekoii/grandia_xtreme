#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetTreasureOpenPuff(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->dirAxis[1] = 1;
    p->count = 0x10;
    p->motionMode = 4;
    p->dirMode = 2;
    p->dirAxis[0] = 0;
    p->dirAxis[2] = 0;
    p->alphaMode = 0;
    p->fadeFrames = 0;
    p->life = 45.0f;
    p->unk12B2 = 0;
    p->sizeRandMode = 0;
    p->spawnRadius = 0.0f;
    p->pointCount = 1;
    p->interval = 0.01f;
    VuVecCopy(p->origin, pos);
    p->dir[0] = 6.4f;
    p->dir[1] = 6.4f;
    p->dir[3] = 0.0f;
    p->dir[2] = 6.4f;
    p->spread[0] = 1.2f;
    p->spread[1] = 1.8f;
    p->spread[2] = 1.2f;
    p->spread[3] = 0.4f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 30.0f;
    p->sizeStart[1] = 30.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeEnd[0] = 90.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[1] = 90.0f;
    p->colorStart[0] = 128.0f;
    p->points = p->unk12CC;
    p->sizeEnd[3] = 0.0f;
    p->colorEnd[0] = 96.0f;
    p->colorStart[1] = 128.0f;
    p->colorStart[2] = 64.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[1] = 88.0f;
    p->colorEnd[2] = 64.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
