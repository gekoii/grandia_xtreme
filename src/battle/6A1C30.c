#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"

void VuVecCopy(f32 *, f32 *, s32);

void FxFailLabelShow(BtlUnit *arg0, s8 arg1) {
    FxFailLabel *p;
    BtlUnit *u;
    s32 one;

    if (arg0->partsChr >= 0) {
        u = &btlUnits[arg0->partsChr];
    } else {
        u = arg0;
    }
    p = &fxFailLabel[u->id];
    p->y = 64.0f;
    p->timer = 0;
    p->vy = 0;
    p->frame = 0;
    if (arg1 == 0) {
        one = 1;
        u->ipSpeed = 0;
        u->scopeType = 0;
        u->ipMode = one;
        u->ipPos = u->ipRet;
        u->flags &= ~0x80u;
        VuVecCopy(u->moveTarget, u->pos, one);
        BtlUnitSetState(u, 0x80);
        u->action = 0x7F;
        u->partsChr = -1;
    }
}
