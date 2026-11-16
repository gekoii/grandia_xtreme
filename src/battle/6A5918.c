#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxWeaponAuraSetKind(BtlUnit *arg0, s8 arg1) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];
    s8 old;

    if ((u8)arg1 <= 16) {
        if (p->kind != arg1) {
            if (p->kind == 0) {
                old = arg0->params->eqpAttr;
                arg0->params->eqpAttr = arg1;
                BtlUnitWeaponFxStart(arg0);
                arg0->params->eqpAttr = old;
            } else if (arg1 == 0) {
                p->stop = 1;
            } else {
                p->kind = arg1;
                ((s32 (*)(void *, s32))PtclBtlSprReset)(p->sprites, 0x18); /* cast: no tail call */
            }
        }
    }
}
