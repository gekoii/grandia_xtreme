/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"
#include "party.h"
#include "protos/battle.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlPathBlockedArcExtend(BtlPathCtx *c, s32 idx, s32 side) {
    BtlUnit *u;
    BtlUnit *t;
    s32 i;
    s32 a, b, v;
    f32 x, z, r;

    u = &btlUnits[idx];
    x = u->pos[0];
    z = u->pos[2];
    r = 1100.0f - u->params->colR;
    if (r * r <= x * x + z * z) {
        if (!side) {
            c->lo = 0xC000;
        } else {
            c->hi = 0x4000;
        }
        return;
    }
    if (c->flags[idx] == 1) {
        f32 dx, dz;
        a = (BtlAtan2Angle(x - c->x, z - c->z) - c->ang) & 0xFFFF;
        dx = u->pos[0] - c->x;
        dz = u->pos[2] - c->z;
        b = BtlAsinAngle((u->params->colR + c->r) / fsqrt(dx * dx + dz * dz)) & 0xFFFF;
        if (!side) {
            v = (a + b) & 0xFFFF;
            if (v > 0xC000) {
                return;
            }
            if (c->lo < v) {
                c->lo = v;
            }
        } else {
            v = (a - b) & 0xFFFF;
            if (v < 0x4000) {
                return;
            }
            if (v < c->hi) {
                c->hi = v;
            }
        }
    }
    c->flags[idx] = 0;
    for (i = 0; i < 16; i++) {
        if (c->flags[i] != 0) {
            f32 dx, dz, d;
            t = &btlUnits[i];
            d = u->params->colR + t->params->colR + (c->r + c->r);
            dx = t->pos[0] - u->pos[0];
            dz = t->pos[2] - u->pos[2];
            if (dx * dx + dz * dz < d * d) {
                BtlPathBlockedArcExtend(c, i, side);
            }
        }
    }
}
