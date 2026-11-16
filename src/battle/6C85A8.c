/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlUnitMoveDelta(BtlUnit *u, s32 flags, f32 dx, f32 dz) {
    f32 push[2];
    PtyUnit *st;

    if (!(u->flags & 2) && u->pos[1] <= 0.0f && u->alpha > 0.0f) {
        st = u->params;
        if (!(st->status & 1)) {
            u->dust -= fsqrt(dx * dx + dz * dz);
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
    push[0] = push[1] = 0.0f;
    if (flags & 2) {
        BtlUnitSeparationPush(u, &push[0], &push[1]);
    }
    if (flags & 1) {
        BtlUnitMoveClamped(u, dx + push[0], dz + push[1]);
    } else {
        u->pos[0] += dx + push[0];
        u->pos[2] += dz + push[1];
    }
}
