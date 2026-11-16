#include "common.h"
#include "battle/n30.h"
#include "protos/battle.h"

void FxWeaponAuraStart(BtlUnit *arg0, s8 arg1, s16 arg2, f32 a, f32 b) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];

    if ((u32)(arg1 - 1) < 16) {
        p->kind = arg1;
        p->unkA = arg2;
        p->unk8 = arg2;
        p->unk3C = a;
        p->unk14 = b;
        p->pause = 1;
        p->seg0[0] = p->seg0[1] = p->seg0[2] = p->seg1[0] = p->seg1[1] = p->seg1[2] = p->dir[0] = p->dir[1] = p->dir[2] = p->unkC = p->stop = p->dirMode = p->unk6 = p->interval = p->timer = p->unk10 = 0;
        PtclBtlSprReset((s8 *)p->sprites, 0x18);
        return;
    }
    p->stop = 1;
}
