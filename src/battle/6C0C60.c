#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void BtlUnitWeaponFxStart(BtlUnit *arg0) {
    f32 a[4];
    f32 b[4];

    if (arg0->params->id == 0x1007) {
        FxWeaponAuraStart(arg0, arg0->params->eqpAttr, 0x302, 1.0f, 0.5f);
        a[0] = -40.0f;
        b[0] = 80.0f;
        b[1] = b[2] = a[1] = a[2] = 0.0f;
        FxWeaponAuraSetSegment(arg0, a, b);
    } else {
        FxWeaponAuraStart(arg0, arg0->params->eqpAttr, 0x366, 1.0f, 0.5f);
        FxWeaponAuraSetFromPoints(arg0, 0, 1, 2.0f);
    }
}
