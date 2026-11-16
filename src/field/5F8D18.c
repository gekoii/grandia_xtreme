/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "pad.h"
#include "field/state.h"

void VuSinCosRad(f32 *, f32 *, f32);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define VUNIT(d) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(d) : "memory")

void ChrPlayerPadMove(ChrObj *o, Pad *pad)
{
    f32 sc[4] __attribute__((aligned(16)));
    Camera *cam;
    s8 x;
    s8 y;
    f32 fx;
    f32 fy;
    f32 m;
    f32 s;
    s32 fl;
    GphModel *p;

    VUNIT(o->vel);
    o->speed = 0.0f;
    if (!(o->flags & 1)) {
        return;
    }
    cam = &camField[0];
    y = 0;
    x = 0;
    if (pad->leftX >= 0xBD) {
        x = -pad->leftX - 0x80;
    }
    if (pad->leftX < 0x44) {
        x = 0x80 - pad->leftX;
    }
    if (pad->leftY >= 0xBD) {
        y = pad->leftY + 0x80;
    }
    if (pad->leftY < 0x44) {
        y = pad->leftY + 0x80;
    }
    s = o->speed;
    if (x != 0 || y != 0) {
        fx = x;
        fy = y;
        m = fsqrt(fx * fx + fy * fy);
        if (m > 0.1f) {
            fx = fx / m;
            fy = fy / m;
        } else {
            fx = s;
            fy = fx;
        }
        fl = o->actFlags;
        if (!(fl & 8)) {
            s = fldState.kamaeSpeed * 2.3088f;
            if (o->stunState == 0 && (fl & 4) && m > 120.0f) {
                s = 3.885f * fldState.runSpeed;
            }
        } else {
            s = fldState.kamaeSpeed * 2.6832f;
        }
        if (SysIsDebugMode() && (pad->held & 2)) {
            s *= 1.7f;
        }
        VuSinCosRad(&sc[0], &sc[1], -cam->yaw);
        o->speed = s;
        o->vel[1] = 0.0f;
        o->vel[0] = (fx * sc[1] + fy * sc[0]) * s * s;
        o->vel[2] = (-fx * sc[0] + fy * sc[1]) * s * s;
    }
    if (SysIsDebugMode()) {
        p = o->model;
        p->pos[1] += (f32)(pad->pressure[2] >> 3);
        p->pos[1] -= (f32)(pad->pressure[3] >> 3);
    }
}
