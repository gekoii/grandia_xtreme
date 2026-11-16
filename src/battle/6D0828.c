#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


void BtlAttackOnHit(BtlUnit *u, s8 flag, BtlUnit *dst) {
    PtyUnit *info;
    s32 v;
    s32 m;
    s32 r;

    if (flag) {
        if (u->partsChr >= 0) {
            u = &btlUnits[u->partsChr];
        }
        info = u->params;
        if (u->id < 4) {
            v = info->sp + (u->action == 0 ? info->spSupCmb : info->spSupCrt);
            info->sp = v;
            if (v >= 0) {
                m = info->maxSp;
                r = (m < v) ? m : v;
            } else {
                r = 0;
            }
            info->sp = r;
        }
        if (info->atkDown > 0) {
            BtlHitQueueAddStatus(dst, 0, 1, 0, info->atkDown);
        }
        if (info->defDown > 0) {
            BtlHitQueueAddStatus(dst, 0, 2, 0, info->defDown);
        }
    }
}
