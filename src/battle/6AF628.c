#include "common.h"
#include "battle/camera.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlCamProcUnitPair(BtlCamCtrl *cam, BtlCamParam *task) {
    BtlUnit *c;
    BtlUnit *o;
    f32 dx, dy, dz, d;

    if (task->phase == 0) {
        cam->to.target[1] = 100.0f;
        task->phase = task->unitPtr->target + 0x100;
        cam->to.rot[2] = 0;
    }
    c = task->unitPtr;
    o = &btlUnits[task->phase & 0xF];
    dx = c->pos[0] - o->pos[0];
    dy = c->pos[1] - o->pos[1];
    dz = c->pos[2] - o->pos[2];
    d = fsqrt(dx * dx + dy * dy + dz * dz);
    cam->to.dist = d * 0.35f + 550.0f;
    cam->to.target[0] = (o->pos[0] + c->pos[0]) * 0.5f;
    cam->to.target[2] = (o->pos[2] + c->pos[2]) * 0.5f;
}
