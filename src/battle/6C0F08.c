/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/n32.h"

BtlActionSet *AiEnemySelectActionSet(BtlUnit *u) {
    PtyUnit *st;
    BtlChrData *d;
    BtlActionSet *a;
    BtlUnit *p;
    BtlUnit *q;
    s32 i;
    s32 j;
    s32 k;
    s32 cnt;
    s8 n;

    st = u->params;
    d = u->def->data;
    i = d->actionSetCount - 1;
    a = &d->actionSets[i];
    for (; i > 0; i--, a--) {
        u->ai = i;
        if (a->type >= 0x28 && a->type < 0x30) {
            if (u->flag & (0x100 << (a->type - 0x28))) {
                return a;
            }
        } else if (a->type >= 0x32 && a->type < 0x3E) {
            if (u->body >= 0) {
                p = &btlUnits[u->body];
            } else {
                p = u;
            }
            switch (a->type) {
            case 0x32:
            case 0x33:
            case 0x34:
                if (btlUnits[p->parts[a->type - 0x32]].params->hp <= 0) {
                    return a;
                }
                break;
            case 0x38:
            case 0x39:
            case 0x3A:
                if (u->flag & (0x40000 << (a->type - 0x38))) {
                    return a;
                }
                break;
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x3B:
            case 0x3C:
            case 0x3D:
                n = 0;
                for (k = 0; k < 3; k++) {
                    if (p->parts[k] < 0) {
                        if (k == n) {
                            n = 3;
                        }
                        break;
                    }
                    if (btlUnits[p->parts[k]].params->hp <= 0) {
                        n++;
                    }
                }
                if (a->type < 0x38) {
                    if (a->type - 0x35 < n) {
                        return a;
                    }
                } else if ((u->flag & 0x1C0000) && a->type - 0x3B < n) {
                    return a;
                }
                break;
            }
        } else {
            switch (a->type) {
            case 1:
                return a;
            case 2:
                if (u->flag & 2) {
                    return a;
                }
                break;
            case 3:
                if (u->turn % 5 == 4) {
                    return a;
                }
                break;
            case 4:
                if (u->turn % 7 == 6) {
                    return a;
                }
                break;
            case 5:
                if (u->turn % 9 == 8) {
                    return a;
                }
                break;
            case 6:
                if (u->turn % 12 == 11) {
                    return a;
                }
                break;
            case 7:
                if (u->flag & 4) {
                    return a;
                }
                break;
            case 8:
                if (st->hp <= st->maxHp * 0.7f) {
                    return a;
                }
                break;
            case 9:
                if (u->flag & 8) {
                    return a;
                }
                break;
            case 10:
                if (st->hp <= st->maxHp * 0.5f) {
                    return a;
                }
                break;
            case 11:
                if (u->flag & 0x10) {
                    return a;
                }
                break;
            case 12:
                if (st->hp <= st->maxHp * 0.3f) {
                    return a;
                }
                break;
            case 13:
                if (u->flag & 0x20) {
                    return a;
                }
                break;
            case 14:
                if (st->status & 0xF) {
                    return a;
                }
                break;
            case 15:
                if (st->status & 0x7FF0) {
                    return a;
                }
                break;
            case 16:
                if (u->powUp & 0x1F00) {
                    return a;
                }
                break;
            case 17:
                if (u->powUp & 0x1F) {
                    return a;
                }
                break;
            case 18:
                if (u->flag & 1) {
                    return a;
                }
                break;
            case 19:
                if ((u->flag & 1) && (st->status & 0x200)) {
                    return a;
                }
                break;
            case 20:
                if ((u->flag & 1) && (st->status & 0x400)) {
                    return a;
                }
                break;
            case 21:
                if (u->flag & 0x40) {
                    return a;
                }
                break;
            case 22:
                if (btlEnemyStartCount / 2 >= btlEnemyAliveCount) {
                    return a;
                }
                break;
            case 23:
                for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                    if (q->params->hp <= q->params->maxHp * 0.5f) {
                        return a;
                    }
                }
                break;
            case 24:
                for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                    if (q->params->status & 0xF) {
                        return a;
                    }
                }
                break;
            case 25:
                for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                    if (q->params->status & 0x7FF0) {
                        return a;
                    }
                }
                break;
            case 26:
                if (u->flag & 0x80) {
                    return a;
                }
                break;
            case 27:
                for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                    if (q->powUp & 0x1F) {
                        return a;
                    }
                }
                break;
            case 28:
            case 29:
            case 30:
                if (btlPartyAliveCount <= 0x1F - a->type) {
                    return a;
                }
                break;
            case 31:
                for (k = 0, q = btlUnits; k < 4; k++, q++) {
                    if (q->params->hp <= q->params->maxHp * 0.5f) {
                        return a;
                    }
                }
                break;
            case 32:
                for (k = 0, q = btlUnits; k < 4; k++, q++) {
                    if (q->params->status == 0) {
                        return a;
                    }
                }
                break;
            case 33:
                for (k = 0, q = btlUnits; k < 4; k++, q++) {
                    if (q->params->status & 0xF) {
                        return a;
                    }
                }
                break;
            case 34:
                for (k = 0, q = btlUnits; k < 4; k++, q++) {
                    if (q->params->status & 0x7FF0) {
                        return a;
                    }
                }
                break;
            case 35:
                for (k = 0, q = btlUnits; k < 4; k++, q++) {
                    if (q->powUp & 0x1F00) {
                        return a;
                    }
                }
                break;
            case 36:
                for (k = 0, q = btlUnits; k < 4; k++, q++) {
                    if (q->powUp & 0x1F) {
                        return a;
                    }
                }
                break;
            case 37:
                if (AiEnemyFindPack(u)) {
                    return a;
                }
                break;
            case 38:
                if (AiEnemyFindPack(u)) {
                    for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                        if (q->params->hp <= q->params->maxHp * 0.5f) {
                            return a;
                        }
                    }
                }
                break;
            case 39:
                if (AiEnemyFindPack(u)) {
                    for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                        if (q != u && q->params->hp <= q->params->maxHp * 0.5f) {
                            return a;
                        }
                    }
                }
                break;
            case 0x30:
                if (u->flag & 0x10000) {
                    for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                        if (q->state >= 0x40) {
                            q->flag &= ~0x10000;
                        }
                    }
                    return a;
                }
                break;
            case 0x31:
                if (u->flag & 0x20000) {
                    return a;
                }
                break;
            case 0x3E:
            case 0x3F:
                cnt = 0;
                for (k = 4, q = btlEnemyUnits; k < 16; k++, q++) {
                    if (q->state >= 0x40 && q != u) {
                        for (j = 0; j < 3; j++) {
                            if (u->parts[j] == k) {
                                break;
                            }
                        }
                        if (j >= 3) {
                            cnt++;
                        }
                    }
                }
                if (cnt < 3 && (a->type == 0x3E || cnt == 0)) {
                    return a;
                }
                break;
            case 0x40:
                if (u->flag < 0) {
                    return a;
                }
                break;
            case 0x41:
                if (u->flag & 0x40000000) {
                    return a;
                }
                break;
            case 0x42:
                if (u->flag & 0x20000000) {
                    return a;
                }
                break;
            case 0x43:
                if (u->params->level >= 10) {
                    return a;
                }
                break;
            case 0x44:
                if (u->params->level >= 25) {
                    return a;
                }
                break;
            case 0x45:
                if (u->params->level >= 40) {
                    return a;
                }
                break;
            case 0x46:
                if (u->params->level >= 55) {
                    return a;
                }
                break;
            case 0x47:
                if ((u->flag & 0x40000000) && !(u->flag & 0x8000000)) {
                    return a;
                }
                break;
            }
        }
    }
    u->ai = 0;
    return a;
}
