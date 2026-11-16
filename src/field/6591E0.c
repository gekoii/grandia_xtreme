/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "protos/field.h"

void PtclEmitterStart(PtclEmitter *);
void VuVecCopy(void *, void *);

#define IS_US() (EvtGetMapNo() == 0x4820 || EvtGetMapNo() == 0x4821 || EvtGetMapNo() == 0x4822)
#define IS_JP() (EvtGetMapNo() == 0x4800 || EvtGetMapNo() == 0x4801 || EvtGetMapNo() == 0x4802)

void PtclPresetFire(PtclEmitter *p, void *pos) {
    f32 r;

    r = VuRandSigned(7.0f);
    if (r < 0.0f) {
        r = -r;
    }
    if (r < 1.0f) {
        r += 1.0f;
    }
    PtclEmitterReset(p);
    p->spawnFlags = 1;
    if (IS_US()) {
        p->motionMode = 1;
    } else {
        p->motionMode = 0;
    }
    p->count = 0x1F;
    p->unk12B2 = -1;
    p->dirMode = 1;
    p->life = 30.0f;
    p->pointCount = 1;
    p->sizeRandMode = 0;
    p->interval = 0.1f;
    if (IS_JP()) {
        p->spawnRadius = r * 20.0f;
    } else {
        p->spawnRadius = 20.0f;
    }
    VuVecCopy(p->origin, pos);
    if (IS_US()) {
        p->dir[0] = 0.55f;
        p->dir[1] = 20.5f;
        p->dir[2] = 0.55f;
        p->dir[3] = 0.0f;
        p->spread[0] = 12.2f;
        p->spread[1] = 0.01f;
        p->spread[2] = 12.2f;
        p->spread[3] = 0.0f;
        p->accel[0] = 0.0f;
        p->accel[1] = -10.0f;
        p->accel[2] = 0.0f;
        p->accel[3] = 0.0f;
    } else if (IS_JP()) {
        p->dir[0] = 0.05f;
        p->dir[1] = r * 2.5f;
        p->dir[2] = 0.05f;
        p->dir[3] = 0.0f;
        p->spread[0] = 12.2f;
        p->spread[1] = 1.6f;
        p->spread[2] = 12.2f;
        p->spread[3] = 0.0f;
        p->accel[0] = 0.0f;
        p->accel[1] = 0.0f;
        p->accel[2] = 0.0f;
        p->accel[3] = 0.0f;
    } else {
        p->dir[0] = 0.05f;
        p->dir[1] = 2.5f;
        p->dir[2] = 0.05f;
        p->dir[3] = 0.0f;
        p->spread[0] = 12.2f;
        p->spread[1] = 1.6f;
        p->spread[2] = 12.2f;
        p->spread[3] = 0.0f;
        p->accel[0] = 0.0f;
        p->accel[1] = 0.0f;
        p->accel[2] = 0.0f;
        p->accel[3] = 0.0f;
    }
    p->unk860[0] = 0.0f;
    p->unk860[1] = 0.0f;
    p->unk860[2] = 0.0f;
    p->unk860[3] = 0.0f;
    if (IS_JP()) {
        p->sizeStart[0] = r * 40.0f;
        p->sizeStart[1] = r * 40.0f;
        p->sizeStart[2] = 0.0f;
        p->sizeStart[3] = 0.0f;
        p->sizeEnd[0] = 1.0f;
        p->sizeEnd[1] = 10.0f;
        p->sizeEnd[2] = 0.0f;
        p->sizeEnd[3] = 0.0f;
    } else {
        p->sizeStart[0] = 40.0f;
        p->sizeStart[1] = 30.0f;
        p->sizeStart[2] = 0.0f;
        p->sizeStart[3] = 0.0f;
        p->sizeEnd[0] = 1.0f;
        p->sizeEnd[1] = 10.0f;
        p->sizeEnd[2] = 0.0f;
        p->sizeEnd[3] = 0.0f;
    }
    p->sizeRandX = 12.0f;
    p->sizeRandY = 12.0f;
    p->unk878 = 0.0f;
    p->unk87C = 0.0f;
    p->points = 0;
    p->colorStart[0] = 180.0f;
    p->colorStart[1] = 64.0f;
    p->colorStart[2] = 32.0f;
    p->colorStart[3] = 128.0f;
    p->colorEnd[0] = 180.0f;
    p->colorEnd[1] = 64.0f;
    p->colorEnd[2] = 32.0f;
    p->colorEnd[3] = 128.0f;
    PtclEmitterStart(p);
}
