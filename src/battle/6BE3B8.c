#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitFixTarget(BtlUnit *p, s32 mode, s32 check) {
    s32 i;

    if (check) {
        switch (mode) {
        case 3:
            if (p->target == p->id) {
                return;
            }
            break;
        case 1:
        case 5:
            if ((p->id < 4 && p->target < 4) || (p->id >= 4 && p->target >= 4)) {
                return;
            }
            break;
        case 6:
            return;
        default:
            if ((p->id < 4 && p->target >= 4) || (p->id >= 4 && p->target < 4)) {
                return;
            }
            break;
        }
    }
    switch (mode) {
    case 3:
        BtlUnitSelectTarget(p, 2, 2);
        break;
    case 1:
        BtlUnitSelectTarget(p, 2, 1);
        break;
    case 5:
        for (i = 0; i < 4; i++) {
            if (btlUnits[i].state == 4) {
                break;
            }
        }
        p->target = i < 4 ? i : p->id;
        break;
    case 6:
        BtlUnitSelectTarget(p, 3, 1);
        break;
    default:
        BtlUnitSelectTarget(p, 1, 1);
        break;
    }
}
