#include "common.h"
#include "battle/unit.h"

void BtlUnitSetPartHook(BtlUnit *arg0, s32 val) {
    BtlUnit *u;
    s32 i;

    if (arg0->body >= 0) {
        u = &btlUnits[arg0->body];
        for (i = 0; i < 3; i++) {
            if (u->parts[i] == arg0->id) {
                break;
            }
        }
        if (i < 3) {
            u->partHooks[i] = val;
        }
    }
}
