#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

void PtclPresetEmberField(PtclEmitter *arg0, void *arg1) {
    f32 temp_f0;
    s32 temp_v0;

    PtclEmitterReset(arg0);
    arg0->count = 0x1F;
    arg0->spawnFlags = 1;
    arg0->motionMode = 1;
    arg0->dirMode = 1;
    arg0->dirAxis[0] = 1;
    arg0->dirAxis[1] = 1;
    arg0->dirAxis[2] = 1;
    arg0->alphaMode = 1;
    arg0->fadeFrames = 0x19;
    arg0->unk12B2 = -1;
    arg0->sizeRandMode = 0;
    arg0->life = 60.0f;
    arg0->pointCount = 0xC;
    arg0->interval = 10.0f;
    arg0->spawnRadius = 1000.0f;
    VuVecCopy(arg0->origin, arg1);
    arg0->dir[0] = 0.2f;
    arg0->dir[1] = 0.8f;
    arg0->dir[3] = 0.0f;
    arg0->spread[3] = 0.0f;
    arg0->dir[2] = 0.2f;
    arg0->accel[0] = 0.0f;
    arg0->spread[0] = 8.0f;
    arg0->spread[1] = 0.8f;
    arg0->accel[1] = 0.0f;
    arg0->spread[2] = 8.0f;
    arg0->accel[2] = 0.0f;
    arg0->accel[3] = 0.0f;
    temp_f0 = VuRandInt(0x1388);
    arg0->unk860[1] = 0.0f;
    arg0->unk860[0] = temp_f0;
    temp_v0 = VuRandInt(0x1388);
    arg0->unk860[3] = 0.0f;
    arg0->sizeStart[2] = 0.0f;
    arg0->sizeStart[3] = 0.0f;
    arg0->sizeEnd[0] = 0.0f;
    arg0->unk860[2] = (f32) temp_v0;
    arg0->sizeEnd[1] = 0.0f;
    arg0->sizeEnd[2] = 0.0f;
    arg0->sizeEnd[3] = 0.0f;
    arg0->sizeStart[0] = 100.0f;
    arg0->unk878 = 0.0f;
    arg0->sizeStart[1] = 100.0f;
    arg0->sizeRandX = 4.0f;
    arg0->sizeRandY = 4.0f;
    arg0->unk87C = 0.0f;
    arg0->points = arg0->unk12CC;
    arg0->colorStart[0] = 255.0f;
    arg0->colorEnd[0] = 255.0f;
    arg0->colorStart[1] = 100.0f;
    arg0->colorEnd[1] = 100.0f;
    arg0->colorStart[2] = 0.0f;
    arg0->colorEnd[2] = arg0->unk860[3];
    arg0->colorStart[3] = 128.0f;
    arg0->colorEnd[3] = 128.0f;
    PtclEmitterStart(arg0);
}

