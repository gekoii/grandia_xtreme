#include "common.h"
#include "battle/n30.h"

void FxWeaponAuraSetDir(BtlUnit *arg0, s8 arg1, f32 x, f32 y, f32 z) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];

    if (p->kind > 0) {
        p->dirMode = arg1;
        p->dir[0] = x;
        p->dir[1] = y;
        p->dir[2] = z;
    }
}
