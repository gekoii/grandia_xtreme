#include "common.h"
#include "battle/n30.h"
#include "protos/battle.h"

void FxWeaponAuraSetInterval(BtlUnit *arg0, u8 arg1, u8 arg2) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];
    f32 r;

    if (p->kind > 0) {
        p->pause = 0;
        p->interval = arg2;
        p->timer = arg2;
        r = (rand() >> 15) * (1.0f / 65536.0f);
        p->unk6 = arg1;
        p->timer = p->timer * r;
    }
}
