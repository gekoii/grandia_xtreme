#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetFallingMotes(PtclEmitter *p, void *pos) {
    double d;

    PtclEmitterReset(p);
    p->spawnFlags = 0;
    p->motionMode = 1;
    p->dirMode = 1;
    p->dirAxis[0] = 1;
    p->dirAxis[1] = 1;
    p->dirAxis[2] = 1;
    p->count = 0x1F;
    p->alphaMode = 1;
    p->fadeFrames = 0x19;
    p->unk12B2 = -1;
    p->life = 300.0f;
    p->sizeRandMode = 0;
    p->pointCount = 8;
    p->spawnRadius = 1000.0f;
    p->interval = 10.0f;
    VuVecCopy(p->origin, pos);
    p->dir[0] = 0.0f;
    p->dir[1] = -1.0f;
    p->dir[2] = 0.0f;
    p->dir[3] = 0.0f;
    p->spread[0] = 0.0f;
    p->spread[1] = 0.3f;
    p->spread[2] = 0.0f;
    p->spread[3] = 0.0f;
    p->accel[0] = 0.0f;
    p->accel[1] = 0.0f;
    d = VuRandSigned(2.4f);
    if (d < 0.0) {
        d = 0.0 - d;
    }
    p->accel[2] = d;
    p->accel[3] = 0.0f;
    p->unk860[0] = VuRandInt(3000);
    p->unk860[1] = VuRandInt(6000);
    p->unk860[2] = VuRandInt(15000);
    p->unk860[3] = 0.0f;
    p->sizeStart[0] = 10.0f;
    p->sizeStart[1] = 10.0f;
    p->sizeStart[2] = 0.0f;
    p->sizeStart[3] = 0.0f;
    p->sizeEnd[0] = 15.0f;
    p->sizeEnd[1] = 15.0f;
    p->sizeEnd[2] = 0.0f;
    p->sizeEnd[3] = 0.0f;
    p->sizeRandX = 20.0f;
    p->sizeRandY = 20.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->points = p->unk12CC;
    p->colorStart[0] = VuRandInt(100) + 100.0f;
    p->colorStart[1] = 200.0f;
    p->colorStart[2] = VuRandInt(40) + 160.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = p->colorStart[0];
    p->colorEnd[1] = 200.0f;
    p->colorEnd[2] = p->colorStart[2];
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
