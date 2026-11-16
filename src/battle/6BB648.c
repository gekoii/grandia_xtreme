/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "grxsys.h"
#include "sys.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/misc.h"

typedef struct {
    s16 x[16];
    s16 z[16];
} Formation;


void BtlUnitsSetup(void) {
    s32 list[4];
    f32 w[4];
    Formation *pos;
    BtlUnit *u;
    BtlUnit *v;
    BtlCharDef *r;
    s32 i;
    s32 j;
    s32 j0;
    s32 j1;
    s32 j2;
    s32 k1;
    s32 k2;
    s32 s1;
    s32 s2;
    s8 c2;
    s32 k;
    s32 n;
    s32 total;
    s32 cnt;
    s32 free;
    s32 per;
    s16 maxP;
    s16 maxE;
    s16 h;
    f32 f;
    f32 step;

    pos = sysGrxsysPac->formations;
    switch (btlEncounterFlags) {
    case 17:
    case 26:
        pos += 6;
        break;
    case 18:
    case 21:
        pos += 7;
        break;
    case 19:
    case 20:
    case 25:
        pos += 8;
        break;
    case 2:
        pos += 9;
        break;
    case 22:
        pos += 10;
        break;
    case 23:
        pos += 11;
        break;
    case 24:
        pos += 12;
        break;
    default:
        switch (btlHandoff.encType) {
        case 1:
            k = 4;
            break;
        case 2:
            k = 2;
            break;
        default:
            k = 0;
            break;
        }
        pos += (s8)(k + ((rand() >> 15) & 1));
        break;
    }

    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        u->id = i;
        u->state = 0;
        u->params = &ptyPartyUnits[i];
        u->def = NULL;
        u->headLook = 0;
        u->wake = 0;
        u->unk1C5 = 0;
    }

    n = 0;
    u = btlUnits;
    for (i = 0; i < 4; i++, u++) {
        if (btlUnitIds.ids[i] != 0 && BtlUnitInitFromDef(u, btlUnitIds.ids[i]) != 0) {
            if (btlFromField != 0) {
                for (j0 = 0; j0 < 2; j0++) {
                    btlTactics[i][j0] = ptyCharData[btlUnitIds.ids[i] - 0x1001].tactics[j0];
                }
            }
            u->ai = btlTactics[i][btlAutoTactics - 1];
            PtyUnitCalcParams(u->params, 0, 0);
            n++;
        }
    }
    btlPartyStartCount = n;
    btlPartyAliveCount = n;

    u = btlUnits;
    for (j = 0; j < 4; j++, u++) {
        if (u->state > 0) {
            PtyUnit *o = u->params;
            if (o->hp <= 0) {
                u->unk188 = 0;
                u->unk18C = 0;
                o->hp = 0;
                o->status = 0x8000;
                u->powUp = 0;
                u->action = 0x7F;
                u->state = 4;
                u->subPhase = 0;
                u->phase = 0;
                ((void (*)(BtlUnit *))u->def->setState)(u);
                u->phase = 2;
                u->rot[1] = rand() >> 15;
                btlPartyAliveCount--;
            }
        }
    }

    u = btlUnits;
    for (i = 0; i < 4; i++, u++) {
        if (u->state > 0) {
            u->moveTarget[0] = u->pos[0] = pos->x[i];
            u->moveTarget[1] = u->pos[1] = 0.0f;
            u->moveTarget[2] = u->pos[2] = pos->z[i];
        }
    }

    u = btlUnits;
    total = 0;
    if (btlPartyAliveCount > 0) {
        s32 *lp = list;
        i = btlPartyAliveCount;
        do {
            if (u->state >= 0x40) {
                i--;
                total += u->params->params[4];
                *lp++ = u->id;
            }
            u++;
        } while (i != 0);
    }

    for (i = 0; i < btlPartyAliveCount; i++) {
        u = &btlUnits[list[i]];
        w[i] = (f32)u->params->params[4] / (f32)total * ((rand() >> 15) * (1.0f / 65536.0f));
    }

    for (i = 0; i < btlPartyAliveCount - 1; i++) {
        for (j1 = i + 1; j1 < btlPartyAliveCount; j1++) {
            if (w[list[i]] < w[list[j1]]) {
                k = list[i];
                list[i] = list[j1];
                list[j1] = k;
            }
        }
    }

    f = 0.125f;
    if (btlPartyAliveCount < 3) {
        f = 0.375f;
    }
    for (i = 0; i < btlPartyAliveCount; i++) {
        u = &btlUnits[list[i]];
        u->ipPos = f;
        IpIconSetPos(u->id, f);
        f += 0.25f;
    }

    btlEnemyAliveCount = 0;
    for (i = 4, u = &btlUnits[4]; i < 16; i++, u++) {
        if (btlUnitIds.ids[i] != 0 && BtlUnitInitFromDef(u, btlUnitIds.ids[i]) != 0) {
            PtyUnitCalcParams(u->params, u->def->data, &ptyBattleParams[u->id - 4]);
            u->exp = u->params->exeSp;
            if (btlUnitIds.flags[i] == 2) {
                u->state = 0;
            } else {
                u->itemRate = (rand() >> 15) * (1.0f / 65536.0f) * 100.0f;
                BtlEnemyDropRoll(u, 0.0f);
                btlEnemyAliveCount++;
            }
        }
    }
    btlEnemyStartCount = btlEnemyAliveCount;

    cnt = 0;
    for (i = 0, r = btlEnemyDefs; i < 4; i++, r++) {
        if (r->id != 0 && r->data->summonId != 0) {
            if (r->data->summonCount == 0) {
                cnt++;
            } else {
                for (k1 = r->data->summonCount; k1 > 0; k1--) {
                    for (s1 = 4; s1 < 16; s1++) {
                        if (btlUnitIds.ids[s1] == 0) {
                            btlUnitIds.ids[s1] = r->data->summonId;
                            btlUnitIds.flags[s1] = 2;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (cnt > 0) {
        free = 0;
        for (i = 4; i < 16; i++) {
            if (btlUnitIds.ids[i] == 0) {
                free++;
            }
        }
        per = free / cnt;
        r = btlEnemyDefs;
        for (i = 0; i < 4; i++, r++) {
            if (r->id != 0 && r->data->summonId != 0 && r->data->summonCount == 0) {
                for (k2 = per; k2 > 0; k2--) {
                    for (s2 = 4; s2 < 16; s2++) {
                        if (btlUnitIds.ids[s2] == 0) {
                            btlUnitIds.ids[s2] = r->data->summonId;
                            btlUnitIds.flags[s2] = 2;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (i = 4, u = &btlUnits[4]; i < 16; i++, u++) {
        if (btlUnitIds.flags[i] == 2 && u->def == NULL && BtlUnitInitFromDef(u, btlUnitIds.ids[i]) != 0) {
            BtlCharDef *d = u->def;
            PtyUnitInitEnemy(u->params, &ptyBattleParams[u->id - 4], (s16)d->id, BtlEnemyGetLevel(d->id), (PtyEnemyData *)u->def->data);
            PtyUnitCalcParams(u->params, u->def->data, &ptyBattleParams[u->id - 4]);
            u->state = 0;
        }
    }

    u = &btlUnits[4];
    for (i = 0; i < 12; i++, u++) {
        if (u->state > 0) {
            if (u->def->data->unk6 > 0) {
                for (j2 = 0; j2 < u->def->data->unk6; j2++) {
                    u->parts[j2] = BtlUnitAttachPart(u->def->id + j2 + 1, u->id);
                }
            }
        }
    }

    u = &btlUnits[4];
    for (i = 4; i < 16; i++, u++) {
        if (u->state > 0) {
            u->moveTarget[0] = u->pos[0] = pos->x[i];
            u->moveTarget[1] = u->pos[1] = 0.0f;
            u->moveTarget[2] = u->pos[2] = pos->z[i];
        }
    }

    f = 0.25f;
    step = 0.5f / btlEnemyAliveCount;
    u = &btlUnits[4];
    for (i = 4; i < 16; i++, u++) {
        if (u->state > 0) {
            u->ipPos = f;
            IpIconSetPos(u->id, f);
            f += step;
        }
    }

    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        if (u->state >= 0x40) {
            BtlUnitSelectTarget(u, 1, 0x1003);
            v = &btlUnits[u->target];
            u->rot[1] = BtlAtan2Angle(v->pos[0] - u->pos[0], v->pos[2] - u->pos[2]);
        }
    }

    if (btlHandoff.encType > 0) {
        if (btlHandoff.encType == 1) {
            u = &btlUnits[4];
            i = 12;
        } else {
            u = btlUnits;
            i = 4;
        }
        for (; i > 0; i--, u++) {
            u->rot[1] = (u->rot[1] + 0x8000) & 0xFFFF;
        }
    }

    btlPartyAvgLevel = 0.0f;
    u = btlUnits;
    maxP = 0;
    maxE = 0;
    c2 = 0;
    for (i = 0; i < 4; i++, u++) {
        if (u->state >= 4) {
            c2++;
            if (maxP < u->params->level) {
                maxP = u->params->level;
            }
            btlPartyAvgLevel += u->params->level;
        }
    }
    btlPartyAvgLevel /= c2;
    for (i = 4; i < 16; i++, u++) {
        if (u->state >= 4) {
            h = u->params->level;
            if (maxE < h) {
                maxE = h;
            }
        }
    }
    btlLevelGap = maxE - maxP;
}
