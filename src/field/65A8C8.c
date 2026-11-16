#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetSmokeSmall(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->motionMode = 1;
    p->dirMode = 2;
    p->dirAxis[1] = 1;
    p->dirAxis[0] = 0;
    p->dirAxis[2] = 0;
    p->count = 0x14;
    p->unk12B2 = 0;
    p->alphaMode = 2;
    p->sizeRandMode = 0;
    p->fadeFrames = 0x19;
    p->life = 45.0f;
    p->pointCount = 1;
    p->spawnRadius = 0.0f;
    p->interval = 0.25f;
    p->points = 0;
    p->origin[0] = 0.0f;
    p->origin[1] = 0.0f;
    p->origin[2] = 0.0f;
    p->origin[3] = 0.0f;
    p->dir[0] = 2.2f;
    p->dir[1] = 0.5f;
    p->dir[2] = 2.2f;
    p->dir[3] = 0.0f;
    p->spread[0] = 1.6f;
    p->spread[1] = 1.0f;
    p->spread[2] = 1.6f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 28.0f;
    p->sizeStart[1] = 28.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 96.0f;
    p->sizeEnd[1] = 96.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
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
