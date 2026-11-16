#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxDrainOrbStart(BtlUnit *arg0, VuVec3 *pos, s32 arg2) {
    FxAtk *e;
    FxDrainOrb *d;
    VuVec3 v;
    s32 i;
    FxDrainSpark *sp;

    if (arg2 < 0) {
        return;
    }
    e = FxAtkAlloc(arg0, 3);
    if (e == NULL) {
        return;
    }
    d = BtlAlloc32(0x320);
    e->work = d;
    if (d == NULL) {
        FxAtkFree(e);
        return;
    }
    e->flags |= 0x100;
    e->update = FxDrainOrbUpdate;
    e->draw = FxDrainOrbDraw;
    d->state = 0;
    d->next = 0;
    d->x = pos->x;
    d->y = pos->y;
    d->z = pos->z;
    d->vy = 15.0f;
    d->speed = 10.0f;
    BtlChrGetPartPos(&v, arg0, 1);
    d->dir = (u16)(BtlAtan2Angle(v.x - d->x, v.z - d->z) - 0x8000);
    d->amount = arg2;
    sp = d->sparks;
    for (i = 23; i >= 0; i--) {
        sp->life = 0;
        sp++;
    }
}
