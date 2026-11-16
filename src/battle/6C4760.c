/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


s32 AiTryHeal(BtlUnit *u, f32 r) {
    BtlUnit *t;
    BtlUnit *p;
    BtlUnit *t2;
    s32 i;
    s32 n;
    f32 dx;
    f32 dz;

    t = &btlUnits[u->target];
    if (t->params->hp <= t->params->maxHp * r) {
        p = btlUnits;
        if (r < 0.45f) {
            if (AiTrySpecialMove(u, AiPickHealSpecial(u, 0), 0)) {
                return 1;
            }
            if (AiTrySpell(u, AiPickHealSpell(u, 0))) {
                return 1;
            }
        } else {
            t = &btlUnits[u->target];
            t2 = t;
            n = 0;
            for (i = 3; i >= 0; i--, p++) {
                if (p->state >= 0x40 && p->params->hp <= p->params->maxHp * r) {
                    dx = t2->pos[0] - p->pos[0];
                    dz = t2->pos[2] - p->pos[2];
                    if (dx * dx + dz * dz < 160000.0f) {
                        n++;
                    }
                }
            }
            if (n >= 2) {
                if (AiTrySpecialMove(u, AiPickHealSpecial(u, 2), 0)) {
                    return 1;
                }
                if (AiTrySpell(u, AiPickHealSpell(u, 2))) {
                    return 1;
                }
            }
            if (AiTrySpecialMove(u, AiPickHealSpecial(u, 1), 0)) {
                return 1;
            }
            if (AiTrySpell(u, AiPickHealSpell(u, 1))) {
                return 1;
            }
        }
    }
    return 0;
}
