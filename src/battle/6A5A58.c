#include "common.h"
#include "battle/n30.h"
#include "protos/battle.h"

void FxWeaponAuraPause(BtlUnit *arg0, s8 arg1) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];

    if (p->kind > 0) {
        if (arg1 != 0) {
            p->pause = -1;
        } else {
            p->pause = 0;
            p->timer = p->interval * ((rand() >> 15) * (1.0f / 65536.0f));
        }
    }
}
