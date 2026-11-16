#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxWeaponAuraDraw(BtlUnit *arg0) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];
    BtlUnit *u;

    if (arg0->body >= 0) {
        u = &btlUnits[arg0->body];
    } else {
        u = arg0;
    }
    if (p->kind > 0 && arg0->state > 0 && !(u->alpha <= 0.0f) && !(u->params->status & 1)) {
        PtclBtlSprDraw(p->sprites, 0x18, p->kind, arg0->alpha);
    }
}
