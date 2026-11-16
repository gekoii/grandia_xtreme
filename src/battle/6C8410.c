/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void BtlUnitMoveDir(BtlUnit *u, s32 ang, s32 flags, f32 spd) {
    f32 v[4];
    PtyUnit *st;

    if (!(u->flags & 2) && u->pos[1] <= 0.0f && u->alpha > 0.0f) {
        st = u->params;
        if (!(st->status & 1)) {
            u->dust -= spd;
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
    v[0] = v[1] = 0.0f;
    if (flags & 2) {
        BtlUnitSeparationPush(u, &v[0], &v[1]);
    }
    VuSinCosAngle(&v[2], &v[3], ang);
    if (flags & 1) {
        BtlUnitMoveClamped(u, spd * v[2] + v[0], spd * v[3] + v[1]);
    } else {
        u->pos[0] += spd * v[2] + v[0];
        u->pos[2] += spd * v[3] + v[1];
    }
}
