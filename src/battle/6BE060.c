#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/n32.h"


void BtlTargetFilterNotTargeted(BtlTargetSel *s, s32 start, s32 n, s32 mask) {
    s8 tmp[16];
    BtlUnit *u;
    PtyMoveEffect *e;
    PtyMoveEffect *e2;
    PtyMoveLevelData *es;
    PtyMoveLevelData *r;
    PtyItemUse *eb;
    PtyMoveLevelData *it;
    s32 i;
    s32 k;
    s32 sum;

    for (i = start; i < n; i++) {
        tmp[i] = s->flags[i];
    }
    u = btlUnits;
    for (k = 0; k < 4; k++, u++) {
        if (u->state >= 0x40 && u->ipMode == 2) {
            e = NULL;
            switch (u->action) {
            case 4:
                it = &ptyMoveTable[u->skillId].levels[u->skillLevel - 1];
                e = &it->effects[it->effectCount - 1];
                break;
            case 2:
                eb = ptyItemTable[u->skillId].use;
                e = &eb->effects[eb->effectCount - 1];
                break;
            case 3:
                r = BtlSpellGetLevelData(u, 0, 0x46);
                e = &r->effects[r->effectCount - 1];
                break;
            }
            if (e == NULL) {
                continue;
            }
            if (mask == 0) {
                if (e->kind != 1) {
                    continue;
                }
            } else if ((u8)(e->kind - 4) < 2 || e->kind == 7 || !(e->flags & mask)) {
                continue;
            }
            tmp[u->target] = 0;
        }
    }
    for (k = 0; k < 12; k++, u++) {
        if (u->state >= 0x40 && u->ipMode == 2) {
            e2 = NULL;
            if (u->comboLeader < 0) {
                es = u->def->data->actionSets[u->ai].actions[u->action]->level;
                e2 = &es->effects[es->effectCount - 1];
            }
            if (e2 == NULL) {
                continue;
            }
            if (mask == 0) {
                if (e2->kind != 1) {
                    continue;
                }
            } else if ((u8)(e2->kind - 4) < 2 || e2->kind == 7 || !(e2->flags & mask)) {
                continue;
            }
            tmp[u->target] = 0;
        }
    }
    if (mask == 0) {
        sum = 0;
        for (i = 0; i < 16; i++) {
            sum += tmp[i];
        }
        if (sum == 0) {
            return;
        }
    }
    for (; start < n; start++) {
        s->flags[start] = tmp[start];
    }
}
