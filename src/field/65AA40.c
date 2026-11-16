#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);

void PtclPresetSparkFountain(PtclEmitter *p, void *pos) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->motionMode = 1;
    p->dirMode = 1;
    p->count = 0x1F;
    p->unk12B2 = 0;
    p->life = 60.0f;
    p->sizeRandMode = 2;
    p->pointCount = 1;
    p->interval = 0.1f;
    p->spawnRadius = 20.0f;
    p->origin[0] = 0.0f;
    p->origin[1] = 0.0f;
    p->origin[2] = 0.0f;
    p->origin[3] = 0.0f;
    p->dir[0] = 1.6f;
    p->dir[1] = 16.0f;
    p->dir[2] = 1.6f;
    p->dir[3] = 0.0f;
    p->spread[0] = 16.0f;
    p->spread[1] = 0.6f;
    p->spread[2] = 16.2f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = -8.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 1.0f;
    p->sizeStart[1] = 1.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 1.0f;
    p->sizeEnd[1] = 1.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandX = 32.0f;
    p->sizeRandY = 32.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->colorStart[0] = 96.0f;
    p->colorStart[1] = 64.0f;
    p->colorStart[2] = 0.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 96.0f;
    p->colorEnd[1] = 64.0f;
    p->colorEnd[2] = 0.0f;
    p->colorEnd[3] = 128.0f;
    p->points = 0;
    PtclEmitterStart(p);
}
