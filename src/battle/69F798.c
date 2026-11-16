/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"
#include "battle/camera.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlCmdMenuPointCamCb(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    BtlCmdMenu *b = &btlCmdMenu;
    BtlUnit *o;
    BtlCmdPoint *e;
    f32 dx, dz, d, t;

    if (arg1->phase == 0) {
        o = arg1->unitPtr;
        e = &b->points[b->unkAC];
        dx = o->pos[0] - e->x;
        dz = o->pos[2] - e->z;
        d = fsqrt(dx * dx + dz * dz);
        t = d / 320.0f;
        arg0->to.rot[2] = 0;
        arg0->to.dist = d * 0.5f + 640.0f;
        arg1->phase++;
        arg0->to.target[0] = (e->x * t + o->pos[0]) / (t + 1.0f);
        arg0->to.target[2] = (e->z * t + o->pos[2]) / (t + 1.0f);
        arg0->to.target[1] = 100.0f;
    }
}
