#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetPurpleWisps(PtclEmitter *p, void *pos, s32 flag) {
    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->motionMode = 1;
    p->count = 0x10;
    p->alphaMode = 2;
    p->dirMode = 1;
    p->dirAxis[0] = 0;
    p->dirAxis[1] = 1;
    p->fadeFrames = 30;
    p->life = 50.0f;
    p->dirAxis[2] = 0;
    p->unk12B2 = 0;
    p->pointCount = 1;
    p->sizeRandMode = 0;
    p->interval = 3.5f;
    p->spawnRadius = 5.0f;
    p->dir[0] = 1.3f;
    p->origin[0] = 0.0f;
    p->origin[1] = 0.0f;
    p->origin[2] = 0.0f;
    p->origin[3] = 0.0f;
    p->dir[3] = 0.0f;
    p->dir[1] = -2.2f;
    p->dir[2] = -12.3f;
    p->spread[0] = 0.2f;
    p->spread[1] = 1.6f;
    p->spread[2] = 0.2f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 1.0f;
    p->accel[2] = 0.0f;
    p->accel[3] = 0.0f;
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 48.0f;
    p->sizeStart[1] = 48.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 128.0f;
    p->sizeEnd[1] = 128.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->points = 0;
    if (flag) {
        p->colorStart[0] = 96.0f;
        p->colorStart[1] = 32.0f;
        p->colorStart[2] = 96.0f;
        p->colorStart[3] = 128.0f;
        p->colorEnd[0] = 96.0f;
        p->colorEnd[1] = 48.0f;
        p->colorEnd[2] = 96.0f;
        p->colorEnd[3] = 128.0f;
    } else {
        p->colorStart[0] = 32.0f;
        p->colorStart[1] = 96.0f;
        p->colorStart[2] = 96.0f;
        p->colorStart[3] = 128.0f;
        p->colorEnd[0] = 48.0f;
        p->colorEnd[1] = 96.0f;
        p->colorEnd[2] = 96.0f;
        p->colorEnd[3] = 128.0f;
    }
    PtclEmitterStart(p);
}
