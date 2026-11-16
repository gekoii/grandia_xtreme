#include "common.h"
#include "save.h"
#include "party.h"
#include "field/state.h"

s32 FldPartyHealFull(FldHpGauge *arg0) {
    PtyUnit *c;
    s32 i;
    s32 ret;

    c = ptyPartyUnits;
    arg0->cur = arg0->max;
    ret = 0;
    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] != 0 && !(c->status & 0x8000)) {
            if (c->hp != c->maxHp) ret = 1;
            c->hp = c->maxHp;
        }
        c++;
    }
    return ret;
}
