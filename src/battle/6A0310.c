/* rodata: 0x006E0DC0-0x006E0DD8 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"

s32 BtlCmdHasTarget(BtlUnit *arg0, s8 arg1) {
    s32 i;
    s32 end;
    BtlUnit *u;

    switch (arg1) {
    case 1:
    case 5:
        if (arg0->id < 4) {
            i = 0;
            end = 4;
        } else {
            i = 4;
            end = 16;
        }
        break;
    case 2:
        if (arg0->id < 4) {
            i = 4;
            end = 16;
        } else {
            i = 0;
            end = 4;
        }
        break;
    case 6:
        i = 0;
        end = 16;
        break;
    case 3:
    case 4:
    default:
        return 1;
    }
    for (u = &btlUnits[i]; i < end; i++, u++) {
        if (arg1 == 5 ? u->state >= 4 : u->state >= 0x40) {
            if (u->id != arg0->id) {
                if (u->params->status & 1) {
                    continue;
                }
            }
            if (!(u->params->bitCsf & 8)) {
                return 1;
            }
        }
    }
    return 0;
}
