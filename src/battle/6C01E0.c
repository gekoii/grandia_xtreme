#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


s32 BtlUnitManaEggSpellCost(BtlUnit *arg0, s32 egg, s32 spell) {
    return PtyManaEggSpellCost(arg0->params, egg, spell);
}
