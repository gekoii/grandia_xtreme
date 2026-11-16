/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"
#include "pad.h"


void BtlUnitHitReact(BtlUnit *u, s32 a1, s32 a2, f32 f) {
    PtyUnit *st;
    BtlUnit *o;
    f32 c, z, v;

    st = u->params;
    if (!(st->bitCsf & 4)) {
        if (u->parts[0] < 0) {
            u->rot[1] = a1;
        }
        st = u->params;
        if (st->antiKb > 0.0f && (rand() >> 15) * (1.0f / 65536.0f) <= st->antiKb) {
            u->knockback = 0.0f;
        } else {
            c = 0.0f;
            v = f;
            z = c;
            if (v > c) {
                do {
                    v -= c;
                    c += 1.0f;
                } while (v > z);
            }
            u->knockback = c;
        }
    } else {
        f = 0.0f;
        u->knockback = f;
    }
    if (btlActingUnit < 0xFF) {
        u->flags |= 8;
    }
    FxTraceStopAll(u->id);
    u->flags &= ~1u;
    u->wake = 0;
    if (u->params->status & 0x20E0) {
        a2 = 0x42;
    }
    u->alpha = 1.0f;
    u->frameSpeed = 1.0f;
    u->evtLoop = 0;
    u->evtCur = 0;
    if (u->state >= 0x80) {
        u->retState = u->state;
        u->retPhase = u->phase;
    }
    if (u->body >= 0) {
        o = &btlUnits[u->body];
        if (o->state >= 0x40) {
            BtlUnitHitReact(o, a1, a2, f);
            BtlUnitSetState(u, 0x7E);
        }
    } else {
        BtlUnitSetState(u, a2);
    }
    if (u->id < 4) {
        if (a2 == 0x42) {
            PadVibrate(padPort0, 1, 0xB4, 0x14);
        } else {
            PadVibrate(padPort0, 0, 0x78, 0x14);
        }
    }
}
