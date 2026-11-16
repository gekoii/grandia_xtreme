#include "common.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/misc.h"

void BtlSchedActFinish(void) {
    BtlUnit *u;
    PtyUnit *st;
    s32 i;
    s32 j;
    PtyManaEgg *p;

    btlActingUnit = 0xFF;
    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        u->flags &= 0xFFFFFFF7;
        if (btlDebugCfg.flags & BTL_DBG_NO_MPSP) {
            st = u->params;
            st->mp = st->maxMp;
            st->sp = st->maxSp;
            p = ptyPartyData->eggs;
            for (j = 24; j > 0; j--, p++) {
                p->mp = p->maxMp;
            }
        }
    }
}
