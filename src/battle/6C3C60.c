#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void AiCmdCritical(BtlUnit *u) {
    PtyUnit *st;
    s32 fl;
    s32 k;
    f32 f;

    if (u->target == u->id || (st = u->params, st->cmdDisable & 0x20)) {
        AiCmdEvade(u);
        return;
    }
    u->action = 1; //P
    u->skillId = u->def->id - 0xF42; //P
    u->skillLevel = 1; //P
    fl = st->bitCsf;
    if (fl & 0x20000) {
        f = 600.0f;
        k = 0x202;
        if (fl & 0x10000) {
            f = 2200.0f;
        }
        u->scopeRadius = f;
        u->scopeType = k;
    } else if (fl & 0x10000) {
        u->scopeType = 0x203;
        u->scopeRadius = 250.0f;
    } else {
        u->scopeType = 0x201;
    }
    BtlUnitSetState(u, 0xC0);
}
