/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "protos/battle.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlCamProcUnitAndTarget(BtlCamCtrl *cam, BtlCamParam *task) {
    BtlUnit *c;
    BtlUnit *o;
    f32 dx, dy, dz, d, t, k, h;

    if (task->phase == 0) {
        c = task->unitPtr;
        o = &btlUnits[c->target];
        dx = c->pos[0] - o->pos[0];
        dy = c->pos[1] - o->pos[1];
        dz = c->pos[2] - o->pos[2];
        d = fsqrt(dx * dx + dy * dy + dz * dz);
        cam->to.dist = d * 0.5f + 640.0f;
        t = d * 0.003125f;
        k = t + 1.0f;
        cam->to.target[0] = (o->pos[0] * t + c->pos[0]) / k;
        cam->to.target[2] = (o->pos[2] * t + c->pos[2]) / k;
        h = BtlChrGetCenterY(c);
        h += BtlChrGetCenterY(o);
        cam->to.rot[2] = 0;
        task->phase++;
        cam->to.target[1] = h * 0.5f + 50.0f;
    }
}
