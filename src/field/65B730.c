#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetFallingSparks(PtclEmitter *arg0, void *arg1) {
    f32 temp_f0;

    PtclEmitterReset(arg0);
    arg0->life = 60.0f;
    arg0->count = 0x1F;
    arg0->dirMode = 2;
    arg0->dirAxis[0] = 0;
    arg0->dirAxis[2] = 0;
    arg0->alphaMode = 0;
    arg0->sizeRandMode = 0;
    arg0->spawnFlags = 1;
    arg0->motionMode = 1;
    arg0->unk12B2 = -1;
    arg0->dirAxis[1] = 1;
    arg0->pointCount = 1;
    arg0->interval = 0.3f;
    arg0->spawnRadius = 2.0f;
    arg0->points = arg0->unk12CC;
    VuVecCopy(arg0->origin, arg1);
    arg0->dir[3] = 0.0f;
    arg0->spread[3] = 0.0f;
    arg0->accel[0] = 0.0f;
    arg0->accel[1] = 0.0f;
    arg0->spread[0] = 10.2f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    arg0->unk860[0] = 0.0f;
    arg0->unk860[1] = 0.0f;
    arg0->unk860[2] = 0.0f;
    arg0->sizeStart[0] = 10.0f;
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[1] = 10.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->sizeRandX = 0.0f;
    arg0->sizeRandY = 0.0f;
    arg0->unk878 = 0.0f;
    arg0->unk87C = 0.0f;
    arg0->sizeEnd[0] = 10.0f;
    arg0->dir[0] = 0.2f;
    arg0->dir[2] = 0.3f;
    arg0->dir[1] = -8.4f;
    arg0->spread[1] = 1.0f;
    arg0->spread[2] = 10.2f;
    arg0->sizeEnd[1] = 10.0f;
    arg0->colorStart[0] = 255.0f;
    arg0->colorStart[1] = 80.0f;
    temp_f0 = VuRandInt(0x28) + 60.0f;
    arg0->colorEnd[0] = arg0->colorStart[0];
    arg0->colorStart[2] = temp_f0;
    arg0->colorEnd[2] = temp_f0;
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[1] = arg0->colorStart[1];
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}

