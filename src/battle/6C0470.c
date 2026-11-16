/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


void BtlUnitLungeStep(BtlUnit *arg0) {
    s32 save[16];
    BtlUnit *t;
    BtlUnit *tgt;
    BtlUnit *u;
    BtlUnitLungeState *p;
    f32 dx;
    f32 dz;
    f32 r;
    f32 spd;
    f32 v;
    s32 i;

    if (arg0->partsChr >= 0) {
        t = &btlUnits[arg0->partsChr];
    } else {
        t = arg0;
    }
    tgt = &btlUnits[t->target];
    dx = arg0->pos[0] - tgt->pos[0];
    dz = arg0->pos[2] - tgt->pos[2];
    r = arg0->params->colR + tgt->params->colR;
    spd = btlUnitLunge[arg0->id].speed;
    if (dx * dx + dz * dz < r * r) {
        spd *= 0.25f;
    }
    if (btlActingUnit != 0xFF) {
        u = btlUnits;
        for (i = 0; i < 16; i++, u++) {
            save[i] = u->params->bitCsf;
            if (i == arg0->id || i == tgt->id) {
                u->params->bitCsf &= 0xFFFFFDFF;
            } else {
                u->params->bitCsf |= 0x200;
            }
        }
        BtlUnitMoveDir(arg0, arg0->rot[1], 3, spd);
        u = btlUnits;
        for (i = 0; i < 16; i++, u++) {
            u->params->bitCsf = save[i];
        }
    } else if (tgt->params->bitCsf & 0x200) {
        tgt->params->bitCsf &= 0xFFFFFDFF;
        BtlUnitMoveDir(arg0, arg0->rot[1], 3, spd);
        tgt->params->bitCsf |= 0x200;
    } else {
        BtlUnitMoveDir(arg0, arg0->rot[1], 3, spd);
    }
    p = &btlUnitLunge[arg0->id];
    v = p->speed - btlUnitLunge[arg0->id].decel;
    p->speed = v;
    if (v < 0.0f) {
        p->speed = 0.0f;
    }
}
