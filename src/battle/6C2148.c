/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"

BtlUnit *AiComboFindPartner(BtlUnit *self, s16 id, s32 min, s8 flag) {
    BtlUnit *u;
    BtlUnit *best;
    f32 bv;
    s32 i;

    bv = 1.0f;
    u = btlEnemyUnits;
    best = NULL;
    for (i = 4; i < 16; i++, u++) {
        if ((aiComboMask >> i) & 1) {
            continue;
        }
        if (u->state < 0x40) {
            continue;
        }
        if (u->params->id != id) {
            continue;
        }
        if (u->params->sp < min) {
            continue;
        }
        if (u->comboLeader >= 0) {
            continue;
        }
        if (u->comboPartners[0] >= 0) {
            continue;
        }
        if (u->params->status & 0x23E0) {
            continue;
        }
        if (u->flags & 0x1000) {
            continue;
        }
        if (flag == 0) {
            return u;
        }
        if (u->ipMode == 2) {
            continue;
        }
        if (u->ipPos <= 0.0f) {
            continue;
        }
        if (bv <= u->ipPos) {
            continue;
        }
        bv = u->ipPos;
        best = u;
    }
    return best;
}
