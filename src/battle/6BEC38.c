/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "protos/battle.h"

#define SQ(x) ((x) * (x))

s32 BtlUnitTurnToTarget(BtlUnit *u) {
    BtlUnit *p;
    BtlUnit *t;
    s32 d;
    s32 fc;
    s32 ang;
    s32 a;
    s32 b;
    s32 r;
    s32 e;

    if (u->partsChr >= 0) {
        p = &btlUnits[u->partsChr];
    } else {
        p = u;
    }
    t = &btlUnits[p->target];
    if (p->target == p->id) {
        return 0;
    }
    if (t->body == u->id) {
        return 0;
    }
    if ((t->params->status & 1) || (t->params->bitCsf & 8)) {
        return 0;
    }

    r = BtlAtan2Angle(t->pos[0] - u->pos[0], t->pos[2] - u->pos[2]);
    ang = u->rot[1];
    d = (u16)(r - ang);
    if (SQ(t->pos[0] - u->pos[0]) + SQ(t->pos[2] - u->pos[2]) <= 1.0f) {
        return 0;
    }
    if (d <= 0x7FFF) {
        if (d < 0x1000) {
            u->turnLeft += d / u->rotAccel;
        } else {
            u->turnLeft += u->rotAccel;
        }
        fc = u->turnLeft;
        if (u->rotVelMax < fc) {
            u->turnLeft = u->rotVelMax;
            fc = u->rotVelMax;
        }

    } else {
        d -= 0x10000;
        if (d >= -0xFFF) {
            u->turnLeft += d / u->rotAccel;
        } else {
            u->turnLeft -= u->rotAccel;
        }
        fc = u->turnLeft;
        if (fc < -u->rotVelMax) {
            u->turnLeft = -u->rotVelMax;
            fc = -u->rotVelMax;
        }
        if (fc < d) {
            u->turnLeft = d;
            fc = d;
        }
    }
    if (fc > 0) {
        e = fc - u->rotAccel / 2;
        u->turnLeft = e;
        if (e >= 0) {
            fc = e;
        } else {
            u->turnLeft = 0;
            fc = 0;
        }

    } else if (fc < 0) {
        e = fc + u->rotAccel / 2;
        u->turnLeft = e;
        if (e > 0) {
            u->turnLeft = 0;
            fc = 0;
        } else {
            fc = e;
        }
    }
    a = d < 0 ? -d : d;
    b = fc < 0 ? -fc : fc;
    if (b >= a) {
        u->rot[1] = (u16)(ang + d);
        return 0;
    }
    u->rot[1] = (u16)(ang + fc);
    d = (BtlAtan2Angle(t->pos[0] - u->pos[0], t->pos[2] - u->pos[2]) - u->rot[1]) & 0xFFFF;
    if (d <= 0x7FFF) {
        return d;
    }
    return d - 0x10000;
}
