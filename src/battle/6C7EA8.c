/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

void VuSinCosAngle(f32 *, f32 *, s32);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 BtlUnitMoveToward(BtlUnit *u, f32 *dst, s32 flags, f32 step, f32 r) {
    f32 v4[4];
    PtyUnit *st;
    s32 ang;
    s32 turn;
    s32 fl;
    s32 cur;
    f32 d;

    if (r < step * 0.5f) {
        r = step * 0.6f;
    }
    /* BtlPathAvoidHeading is (BtlUnit *, f32 *, s32, f32); called float-first for the EABI argument-load order */
    ang = ((s32 (*)(BtlUnit *, f32 *, f32, s32))BtlPathAvoidHeading)(u, dst, r, flags);
    if (ang > 0x3FFFF) {
        return 1;
    }
    if (ang > 0xFFFF) {
        ang -= 0x1F40;
    }
    fl = u->flags;
    if (fl & 0x100) {
        turn = 0;
    } else {
        d = (f32)u->rotVel * step;
        turn = (s32)d + 0x100;
        {
            f32 dx, dz;
            dx = dst[0] - u->pos[0];
            dz = dst[2] - u->pos[2];
            d = fsqrt(dx * dx + dz * dz);
        }
        if (d < r + 100.0f) {
            turn = (f32)turn + (f32)(0x4000 - turn) * ((100.0f - (d - r)) * 0.01f);
        }
    }
    cur = u->rot[1];
    ang = (ang - cur) & 0xFFFF;
    if (ang <= 0x7FFF) {
        if (ang < turn) {
            u->rot[1] = (u16)(cur + ang);
        } else {
            u->rot[1] = (u16)(cur + turn);
        }
    } else if (0x10000 - turn < ang) {
        u->rot[1] = (cur - (0x10000 - ang)) & 0xFFFF;
    } else {
        u->rot[1] = (cur - turn) & 0xFFFF;
    }
    if (!(fl & 2) && u->pos[1] <= 0.0f && u->alpha > 0.0f) {
        st = u->params;
        if (!(st->status & 1)) {
            u->dust -= step;
            if (u->dust < 0.0f) {
                if (st->bitCsf & 0x100) {
                    PtclBtlDustSpawn(0x83, u, u->pos[0], u->pos[2]);
                    u->dust = 50.0f;
                } else {
                    PtclBtlDustSpawn(1, NULL, u->pos[0], u->pos[2]);
                    u->dust = 30.0f;
                }
            }
        }
    }
    v4[0] = v4[1] = 0.0f;
    if (flags & 2) {
        BtlUnitSeparationPush(u, &v4[0], &v4[1]);
    }
    VuSinCosAngle(&v4[2], &v4[3], u->rot[1]);
    if (flags & 1) {
        BtlUnitMoveClamped(u, step * v4[2] + v4[0], step * v4[3] + v4[1]);
    } else {
        u->pos[0] += step * v4[2] + v4[0];
        u->pos[2] += step * v4[3] + v4[1];
    }
    {
        f32 dx, dz;
        dx = dst[0] - u->pos[0];
        dz = dst[2] - u->pos[2];
        if (dx * dx + dz * dz <= r * r) {
            return 1;
        }
    }
    return 0;
}
