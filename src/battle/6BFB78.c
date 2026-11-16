#include "common.h"
#include "battle/unit.h"

void BtlUnitSetTargetArea(BtlUnit *u, s8 type, s8 mode) {
    u->scopeType = type;
    switch ((s8)(mode - 1)) {
    case 4:
        u->scopeType |= 0x400;
        /* fallthrough */
    case 0:
    case 2:
        u->scopeType |= 0x100;
        return;
    case 1:
        u->scopeType |= 0x200;
        return;
    case 3:
        u->scopeType |= 0x300;
        /* fallthrough */
    default:
        return;
    }
}
