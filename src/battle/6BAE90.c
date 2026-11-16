/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitPartSyncAction(BtlUnit *u) {
    BtlUnit *p;
    s32 st;
    s32 found;
    s32 i;
    s8 *t;

    p = u;
    st = u->state;
    if (st >= 0x40 && u->ipMode == 2 && u->ipPos <= 0.0f) {
        found = 1;
    } else {
        i = 0;
        t = u->parts;
        for (; i < 3; i++) {
            if (t[i] >= 0) {
                p = &btlUnits[t[i]];
                if (p->state >= 0x40 && p->ipMode == 2 && p->ipPos <= 0.0f && p->action != 6) {
                    break;
                }
            }
        }
        found = 0;
        if (i < 3) found = 1;
    }
    if (u->partsChr < 0) {
        if (!found) {
            return;
        }
        if ((u->state == 0x80 || u->state == 0xC0) && p->state == 0xC0) {
            switch (BtlUnitCommandValidate(p)) {
            case 1:
                u->rtAct = u->state;
                u->rtP = u->phase;
                u->partsChr = p->id;
                BtlUnitSetState(u, 0xC1);
                break;
            case 2:
                BtlSchedActRequest(u->id);
                u->rtAct = u->state;
                u->rtP = u->phase;
                u->partsChr = p->id;
                break;
            default:
                return;
            }
        }
    } else {
        p = &btlUnits[u->partsChr];
        if (u->state == 0x80 || u->state == 0xC0) {
            if (btlActingUnit == u->id) {
                if (BtlUnitCommandValidate(p) != 0) {
                    BtlUnitActionBegin(p);
                    u->rtAct = u->state;
                    u->rtP = u->phase;
                    BtlUnitSetState(u, 0xC1);
                } else {
                    BtlSchedActDone(u->id);
                    u->partsChr = -1;
                    return;
                }
            } else {
                u->partsChr = -1;
            }
        }
    }
    if (found) {
        u->ipStop = 1;
        for (i = 0; i < 3; i++) {
            if (u->parts[i] >= 0) {
                btlUnits[u->parts[i]].ipStop = 1;
            }
        }
    }
}
