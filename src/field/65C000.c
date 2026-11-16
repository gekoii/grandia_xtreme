#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void *FldMemAlloc(s32);
void PtclEmitterStart(PtclEmitter *);
void MemSet(void *, s32, s32);

void PtclDustInit(void) {
    PtclEmitter *cam;
    PtclDustWork *p;

    p = FldMemAlloc(0x1500);
    ptclDustWork = p;
    MemSet(p, 0, 0x1500);
    cam = &ptclDustWork->emitter;
    PtclEmitterReset(cam);
    cam->count = 0x1F;
    cam->life = 40.0f;
    cam->interval = 0.1f;
    cam->unk12B2 = -1;
    cam->pointCount = 1;
    cam->spawnFlags = 0;
    cam->motionMode = 0;
    cam->dirMode = 0;
    cam->dirAxis[0] = 0;
    cam->dirAxis[1] = 0;
    cam->dirAxis[2] = 0;
    cam->sizeRandMode = 0;
    cam->spawnRadius = 0.0f;
    cam->origin[0] = 0.0f;
    cam->origin[1] = 0.0f;
    cam->origin[2] = 0.0f;
    cam->origin[3] = 0.0f;
    cam->dir[0] = 0.0f;
    cam->dir[1] = 0.0f;
    cam->dir[2] = 0.0f;
    cam->dir[3] = 0.0f;
    cam->spread[0] = 0.0f;
    cam->spread[1] = 0.0f;
    cam->spread[2] = 0.0f;
    cam->spread[3] = 0.0f;
    cam->accel[0] = 0.0f;
    cam->accel[1] = 0.0f;
    cam->accel[2] = 0.0f;
    cam->accel[3] = 0.0f;
    cam->unk860[0] = 0.0f;
    cam->unk860[1] = 0.0f;
    cam->unk860[2] = 0.0f;
    cam->unk860[3] = 0.0f;
    cam->sizeStart[0] = 0.0f;
    cam->sizeStart[1] = 0.0f;
    cam->sizeStart[2] = 0.0f;
    cam->sizeStart[3] = 0.0f;
    cam->sizeEnd[0] = 0.0f;
    cam->sizeEnd[1] = 0.0f;
    cam->sizeEnd[2] = 0.0f;
    cam->sizeEnd[3] = 0.0f;
    cam->sizeRandX = 0.0f;
    cam->sizeRandY = 0.0f;
    cam->unk878 = 0.0f;
    cam->unk87C = 0.0f;
    cam->points = (FMATRIX *)ptclDustWork->emitter.unk12C0;
    cam->colorStart[0] = 120.0f;
    cam->colorStart[1] = 105.0f;
    cam->colorStart[2] = 75.0f;
    cam->colorStart[3] = 128.0f;
    cam->colorEnd[0] = 120.0f;
    cam->colorEnd[1] = 105.0f;
    cam->colorEnd[2] = 75.0f;
    cam->colorEnd[3] = 128.0f;
    PtclEmitterStart(cam);
    cam->unk12B9 = 1;
    cam->unk12A8 = 800000.0f;
}

