/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"

#define RAND() ((rand() >> 15) * (1.0f / 65536.0f))

/* Three parameters: 0x0069E008 passes a fourth (1) through a cast; every other caller passes three. */
void BtlUnitSelectTarget(BtlUnit *u, s32 arg1, s32 cmd) {
    BtlTargetSel sel;
    BtlUnit *v;
    s32 type;
    s32 start;
    s32 end;
    s32 i;
    s32 cnt;
    s32 me;
    s32 useRand;
    f32 r;
    f32 step;

    useRand = cmd < 0x1000;
    type = arg1 & 0xF;
    cmd &= 0xFF;
    if (type == 3) {
        start = 0;
        end = 16;
    } else if ((type == 2 && u->id < 4) || (type == 1 && u->id >= 4)) {
        start = 0;
        end = 4;
    } else {
        start = 4;
        end = 16;
    }
    r = RAND();
    for (i = 0; i < 16; i++) {
        sel.flags[i] = 0;
    }
    v = &btlUnits[start];
    for (i = start; i < end; i++, v++) {
        if (v->state >= 0x40) {
            if (v->id != u->id && (v->params->status & 1)) {
                continue;
            }
            if (!(v->params->bitCsf & 8)) {
                sel.flags[i] = 1;
            }
        }
    }
    if (arg1 & 0x10) {
        me = u->id;
        sel.flags[me] = 0;
        v = &btlUnits[start];
        for (i = start; i < end; i++, v++) {
            if (v->body >= 0 && (me == v->body || u->body == v->body)) {
                sel.flags[v->id] = 0;
            }
        }
        if (u->body >= 0) {
            sel.flags[u->body] = 0;
        }
    }
    if (type == 1 && BtlTargetFilterByRate(&sel, start, end) != 0) {
        cmd = 1;
    } else if (cmd >= 0x24 && cmd < 0x2E) {
        switch (cmd) {
        case 0x24:
            BtlTargetFilterByStatMod(&sel, start, end, 0x100);
            break;
        case 0x25:
            BtlTargetFilterByStatMod(&sel, start, end, 1);
            break;
        case 0x26:
            BtlTargetFilterByStatMod(&sel, start, end, 0x200);
            break;
        case 0x27:
            BtlTargetFilterByStatMod(&sel, start, end, 2);
            break;
        case 0x28:
            BtlTargetFilterByStatMod(&sel, start, end, 0x400);
            break;
        case 0x29:
            BtlTargetFilterByStatMod(&sel, start, end, 4);
            break;
        case 0x2A:
            BtlTargetFilterByStatMod(&sel, start, end, 0x800);
            break;
        case 0x2B:
            BtlTargetFilterByStatMod(&sel, start, end, 8);
            break;
        case 0x2C:
            BtlTargetFilterByStatMod(&sel, start, end, 0x10);
            break;
        default:
            BtlTargetFilterByStatMod(&sel, start, end, 0x10);
            break;
        }
        cmd = 1;
    } else if (cmd >= 0x2E && cmd < 0x37) {
        switch (cmd) {
        case 0x2E:
            BtlTargetFilterByStatus(&sel, start, end, 0xF, 0);
            break;
        case 0x2F:
            BtlTargetFilterNotTargeted(&sel, start, end, 0x7FF0);
            BtlTargetFilterByStatus(&sel, start, end, 0x7FF0, 0);
            break;
        case 0x30:
            BtlTargetFilterByStatus(&sel, start, end, 0x7FF0, 1);
            break;
        case 0x31:
            BtlTargetFilterNotTargeted(&sel, start, end, 0x10);
            BtlTargetFilterByStatus(&sel, start, end, 0x10, 0);
            break;
        case 0x32:
            BtlTargetFilterByStatus(&sel, start, end, 0x10, 1);
            break;
        case 0x33:
            BtlTargetFilterNotTargeted(&sel, start, end, 0x1000);
            BtlTargetFilterByStatus(&sel, start, end, 0x1000, 0);
            break;
        case 0x34:
            BtlTargetFilterByStatus(&sel, start, end, 0x1000, 1);
            break;
        case 0x35:
            BtlTargetFilterNotTargeted(&sel, start, end, 0x800);
            BtlTargetFilterByStatus(&sel, start, end, 0x800, 0);
            break;
        default:
            BtlTargetFilterByStatus(&sel, start, end, 0x800, 1);
            break;
        }
        cmd = 1;
    }
    cnt = 0;
    for (i = start; i < end; i++) {
        if (sel.flags[i] != 0) {
            cnt++;
        }
    }
    if (cnt == 0) {
        u->target = u->id;
        return;
    }
    step = 1.0f / cnt;
    for (i = start; i < end - 1; i++) {
        if (sel.flags[i] != 0) {
            r -= step;
            if (r <= 0.0f) {
                break;
            }
        }
    }
    u->target = i;
    sel.self = u;
    sel.idx = -1;
    sel.prob = 0.6f;
    if (cmd >= 0x14 && cmd < 0x24) {
        BtlTargetPickByStat(&sel, start, end, &u->params->regMgcAttr[(cmd - 0x14) / 2 + 1], !(cmd & 1) ? 0x106 : 6);
    } else {
        switch (cmd) {
        case 2:
            if (sel.flags[u->id] != 0) {
                u->target = u->id;
            }
            return;
        case 3:
            BtlTargetPickByDistance(&sel, start, end, 0);
            sel.prob = 0.4f;
            break;
        case 4:
            BtlTargetPickByDistance(&sel, start, end, 1);
            sel.prob = 0.4f;
            break;
        case 5:
            BtlTargetPickByIp(&sel, start, end, 1.5f);
            break;
        case 6:
            BtlTargetPickByIp(&sel, start, end, 1.0f);
            break;
        case 7:
            BtlTargetPickByIp(&sel, start, end, 0.0f);
            break;
        case 8:
            if (sel.flags[u->attacker] != 0 && RAND() < sel.prob) {
                u->target = u->attacker;
            }
            return;
        case 9:
            BtlTargetPickByCrowding(&sel, start, end, 0);
            sel.prob = 0.4f;
            break;
        case 10:
            BtlTargetPickByCrowding(&sel, start, end, 1);
            sel.prob = 0.4f;
            break;
        case 11:
            BtlTargetPickByStat(&sel, start, end, &u->params->level, 1);
            sel.prob = 0.4f;
            break;
        case 12:
            BtlTargetPickByStat(&sel, start, end, &u->params->hp, 0);
            break;
        case 13:
            BtlTargetFilterNotTargeted(&sel, start, end, 0);
            BtlTargetPickByStat(&sel, start, end, &u->params->hp, 0x100);
            break;
        case 14:
            BtlTargetPickByStat(&sel, start, end, &u->params->params[0], 0);
            sel.prob = 0.4f;
            break;
        case 15:
            BtlTargetPickByStat(&sel, start, end, &u->params->params[1], 0);
            sel.prob = 0.4f;
            break;
        case 16:
            BtlTargetPickByStat(&sel, start, end, &u->params->params[2], 0);
            sel.prob = 0.4f;
            break;
        case 17:
            BtlTargetPickByStat(&sel, start, end, &u->params->params[3], 0);
            sel.prob = 0.4f;
            break;
        case 18:
            BtlTargetPickByStat(&sel, start, end, &u->params->params[4], 0);
            sel.prob = 0.4f;
            break;
        case 19:
            BtlTargetPickByStat(&sel, start, end, &u->params->params[5], 0);
            sel.prob = 0.4f;
            break;
        case 0x7E:
            BtlTargetPickByRank(&sel, start, end, 1);
            break;
        case 0x7F:
            BtlTargetFilterNearGroup(&sel, start, end, 400.0f);
            BtlTargetPickByStat(&sel, start, end, &u->params->hp, 0x100);
            break;
        case 1:
        default:
            return;
        }
    }
    if (!useRand || RAND() < sel.prob) {
        if (sel.idx >= 0) {
            u->target = sel.idx;
        }
    }
}
