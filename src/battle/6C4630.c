#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


s32 AiTryRevive(BtlUnit *u) {
    s32 i;
    s32 j;
    BtlUnit *p;

    i = 0;
    p = btlUnits;
    do {
        if (p->state == 4) {
            for (j = 0; j < 4; j++) {
                if (btlUnits[j].state >= 0x40 && btlUnits[j].target == i) {
                    break;
                }
            }
            if (j >= 4) {
                break;
            }
        }
        i++;
        p++;
    } while (i < 4);
    if (i < 4) {
        u->target = i;
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 0, 1, 0x8000, 1), 0)) {
            return 1;
        }
        if (AiTrySpell(u, AiPickStatusSpell(u, 0, 1, 0x8000))) {
            return 1;
        }
    }
    return 0;
}
