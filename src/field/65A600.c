#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetSteamSlow(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->dirMode = 2;
    p->dirAxis[0] = 0;
    p->dirAxis[2] = 0;
    p->motionMode = 1;
    p->alphaMode = 0;
    p->dirAxis[1] = 1;
    p->count = 0x10;
    p->unk12B2 = -1;
    p->spawnRadius = 0.0f;
    p->life = 180.0f;
    p->sizeRandMode = 1;
    p->pointCount = 1;
    p->interval = 10.0f;
    p->dir[1] = 2.0f;
    p->dir[3] = 0.0f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->dir[0] = 0.12f;
    p->accel[1] = 0.0f;
    p->dir[2] = 0.12f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->spread[0] = 5.2f;
    p->spread[2] = 5.2f;
    p->unk860[0] = 0.0f;
    p->spread[1] = 0.001f;
    p->sizeStart[0] = 20.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[1] = 20.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 100.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandY = 0.0f;
    p->sizeEnd[1] = 100.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->points = 0;
    p->colorStart[0] = 160.0f;
    p->sizeRandX = 5.0f;
    p->colorStart[1] = 160.0f;
    p->colorStart[2] = 160.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 160.0f;
    p->colorEnd[1] = 160.0f;
    p->colorEnd[2] = 160.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
