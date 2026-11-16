#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void BtlUnitAddManaEggMp(BtlUnit *arg0, s32 arg1) {
    if (arg0->id < 4) {
        PtyManaEggsAddMp(&ptyCharData[arg0->def->id - 0x1001], arg1, ptyCharDefTable[arg0->params->id - 0x1001].eggSlots);
    }
}
