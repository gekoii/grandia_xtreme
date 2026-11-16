#include "common.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/n32.h"

s32 AiEnemyFindPack(BtlUnit *arg0) {
    BtlUnit *u;
    s32 n;
    s8 v;

    aiComboMask = 1 << arg0->id;
    v = arg0->def->data->unk3;
    if (v >= 8) {
        n = 2;
    } else if (v >= 4) {
        n = 3;
    } else {
        n = 4;
    }
    for (; n >= 2; n--) {
        u = AiComboFindPartner(arg0, arg0->params->id, 0, 0);
        if (u == NULL) {
            return 0;
        }
        aiComboMask |= 1 << u->id;
    }
    return 1;
}
