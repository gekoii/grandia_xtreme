#include "common.h"
#include "battle/unit.h"
#include "party.h"

s32 BtlEnemyGetLevel(s32 id) {
    BtlUnit *u = btlEnemyUnits;
    s32 i;

    for (i = 4; i < 16; i++, u++) {
        if (u->def != NULL && u->def->id == id) {
            return u->params->level;
        }
    }
    return 1;
}
