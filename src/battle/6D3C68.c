/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/misc.h"
#include "party.h"


void BtlHudComboPopupAdd(s32 idx) {
    VuVec4 v;
    BtlHudUnitGauge *g;
    BtlHudComboPopup *p;
    BtlHudComboPopup *q;
    BtlUnit *u;
    s32 i;
    s32 max;

    g = &btlHudUnitGauges[idx];
    if (g->hits >= 2 || (idx >= 4 && g->hits > 0 && (btlEncounterFlags & 0x10))) {
        if (idx >= 4) {
            p = btlHudEnemyComboPopups;
        } else {
            p = btlHudPartyComboPopups;
        }
        for (i = 0; i < 12; i++, p++) {
            if (p->rank == 0) {
                break;
            }
        }
        if (i >= 12) {
            if (g->timeout <= 0) {
                g->hits = -1;
            } else {
                g->hits = 0;
            }
            return;
        }
        {
            max = 0;
            if (idx >= 4) {
                q = btlHudEnemyComboPopups;
            } else {
                q = btlHudPartyComboPopups;
            }
            for (i = 0; i < 12; i++, q++) {
                if (max < q->rank) {
                    max = q->rank;
                }
            }
            p->timer = 0;
            p->rank = max + 1;
            u = &btlUnits[idx];
            p->hits = g->hits;
            p->damage = g->damage;
            p->unit = idx;
            p->icon = u->params->icon;
            p->hitsWidth = MenuNumberWidth(&btlHudPopupHitsNumFmt, p->hits);
            p->damageWidth = MenuNumberWidth(&btlHudPopupDamageNumFmt, p->damage);
            BtlChrGetPartPos(&v, u, 1);
            UtilWorldToScreen(&v, &v);
            v.x += -41.0f;
            v.y += 48.0f;
            if (idx >= 4) {
                p->destX = 44.0f;
            } else {
                p->destX = 596.0f - (p->hitsWidth + 14.0f + p->damageWidth + 56.0f);
            }
            p->destY = p->rank * 36.0f + 80.0f;
            if (v.x < -256.0f || v.x > 896.0f || v.y < -64.0f || v.y > 512.0f) {
                if (idx >= 4) {
                    p->startX = -256.0f;
                } else {
                    p->startX = 256.0f;
                }
                p->startY = p->destY;
            } else {
                p->startX = v.x;
                p->startY = v.y;
            }
            p->x = p->startX;
            p->y = p->startY;
        }
    }
    if (g->timeout <= 0) {
        g->hits = -1;
    } else {
        g->hits = 0;
    }
}
