#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void BtlResultLevelUpPop(BtlResultMember *o) {
    SndSePlay3D(0x7DB, 0, 0, NULL);
    o->pops[o->popNext].t = 1.5f;
    o->pops[o->popNext].x = o->x + 110.0f - 34.0f;
    o->pops[o->popNext].y = o->y + 92.0f - 16.0f;
    if (++o->popNext >= 8) {
        o->popNext = 0;
    }
}
