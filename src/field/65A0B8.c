#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetTreasureDarkPuff(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->motionMode = 1;
    p->dirMode = 2;
    p->dirAxis[0] = 0;
    p->dirAxis[2] = 0;
    p->count = 0x1F;
    p->unk12B2 = 0;
    p->alphaMode = 2;
    p->dirAxis[1] = 1;
    p->sizeRandMode = 0;
    p->fadeFrames = 0x19;
    p->life = 60.0f;
    p->pointCount = 1;
    p->spawnRadius = 0.0f;
    p->interval = 0.25f;
    VuVecCopy(p->origin, pos);
    p->dir[0] = 3.0f;
    p->dir[1] = 2.2f;
    p->dir[2] = 3.0f;
    p->spread[1] = 1.6f;
    p->spread[0] = 2.0f;
    p->sizeStart[0] = 28.0f;
    p->dir[3] = 0.0f;
    p->spread[2] = 2.0f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->sizeStart[1] = 28.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 96.0f;
    p->sizeEnd[1] = 96.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->points = p->unk12CC;
    p->colorStart[0] = 64.0f;
    p->colorStart[1] = 32.0f;
    p->colorStart[2] = 64.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 32.0f;
    p->colorEnd[1] = 32.0f;
    p->colorEnd[2] = 64.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
