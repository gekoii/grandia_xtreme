/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"

f32 BtlChrGetHeight(BtlUnit *);
static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlCamProcTargets(BtlCamCtrl *cam, BtlCamParam *task) {
    f32 minx, maxx, minz, maxz, maxh, h;
    f32 d, dmax;
    BtlUnit *c;
    BtlUnit *t;
    u32 m;
    BtlUnit *c2;
    u32 m2;

    if (task->phase == 0) {
        minz = 99999.9f;
        maxz = -99999.9f;
        c = &btlUnits[15];
        maxh = 0.0f;
        minx = 99999.9f;
        maxx = -99999.9f;
        for (m = 0x8000; m != 0; m >>= 1, c--) {
            if (btlTargetMask & m) {
                if (c->pos[0] < minx) minx = c->pos[0];
                if (maxx < c->pos[0]) maxx = c->pos[0];
                if (c->pos[2] < minz) minz = c->pos[2];
                if (maxz < c->pos[2]) maxz = c->pos[2];
                h = BtlChrGetHeight(c);
                if (maxh < h) maxh = h;
            }
        }
        t = task->unitPtr;
        /* volatile reads: state is loaded again at each of the three tests */
        if (*(volatile s32 *)&t->state == 0x81) {
            if (t->pos[0] < minx) minx = t->pos[0];
            if (maxx < t->pos[0]) maxx = t->pos[0];
            if (t->pos[2] < minz) minz = t->pos[2];
            if (maxz < t->pos[2]) maxz = t->pos[2];
        }
        cam->to.target[1] = maxh * 0.5f;
        cam->to.target[0] = (minx + maxx) * 0.5f;
        cam->to.target[2] = (minz + maxz) * 0.5f;
        dmax = 0.0f;
        c2 = &btlUnits[15];
        for (m2 = 0x8000; m2 != 0; m2 >>= 1, c2--) {
            if (btlTargetMask & m2) {
                f32 dx = c2->pos[0] - cam->to.target[0];
                f32 dz = c2->pos[2] - cam->to.target[2];
                f32 dd = dx * dx + dz * dz;

                if (dmax < dd) dmax = dd;
            }
        }
        c = task->unitPtr;
        if (*(volatile s32 *)&t->state == 0x81) {
            f32 dx = c->pos[0] - cam->to.target[0];
            f32 dz = c->pos[2] - cam->to.target[2];
            f32 dd = dx * dx + dz * dz;

            if (dmax < dd) dmax = dd;
        }
        d = fsqrt(dmax);
        cam->to.dist = d * 1.1f + 800.0f;
        if (*(volatile s32 *)&t->state != 0x81) {
            cam->to.rot[0] = 0x3400;
            cam->to.rot[1] = 0x8000;
            cam->to.rot[2] = 0;
        }
        task->phase++;
    }
}
