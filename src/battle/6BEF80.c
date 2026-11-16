/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/misc.h"
#include "battle/n32.h"

#define SQ(x) ((x) * (x))

void VuSinCosAngle(f32 *, f32 *, s32);

s32 BtlUnitCheckReach(BtlUnit *u) {
    BtlUnit *p;
    BtlUnit *t;
    PtyMove *pt;
    BtlAction *o;
    s32 k;
    f32 v[2];
    f32 r;
    f32 rad;
    f32 h;
    f32 x;
    f32 z;

    if (u->partsChr >= 0) {
        p = &btlUnits[u->partsChr];
    } else {
        p = u;
    }
    if (btlPhase != 0x80) {
        return 0;
    }
    if (u->id < 4) {
        pt = &ptyMoveTable[u->skillId];
        r = pt->reach;
        h = pt->levels[u->skillLevel - 1].scope;
    } else {
        k = p->def->type;
        o = p->def->data->actionSets[p->ai].actions[p->action];
        if (k < 7) {
            if (k < 2) {
                r = o->reach * 0.8f;
            } else {
                r = 0.0f;
            }
        } else {
            r = o->reach * 0.8f;
        }
        h = o->level->scope;
    }
    VuSinCosAngle(&v[0], &v[1], u->rot[1]);
    rad = u->params->colR + r;
    x = rad * v[0] + u->pos[0];
    z = rad * v[1] + u->pos[2];
    DbgBtlHitMarkAdd(x, z, h);
    t = &btlUnits[p->target];
    if (SQ(x - t->pos[0]) + SQ(z - t->pos[2]) <= SQ(h + t->params->colR)) {
        if ((u8)(p->def->type - 2) >= 3) {
            BtlHudMarkerSpawn(p, 3);
        }
        return 1;
    }
    if ((u8)(p->def->type - 2) >= 3) {
        BtlHudMarkerSpawn(p, 2);
    }
    return 0;
}
