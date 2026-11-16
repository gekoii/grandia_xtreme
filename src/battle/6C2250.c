#include "common.h"
#include "battle/unit.h"
#include "battle/n32.h"

s32 AiComboJoin(BtlUnit *arg0, BtlAction *g) {
    s32 i;
    s8 k;

    if (AiComboFindPartners(arg0, g, 1) == 0) {
        return 0;
    }
    k = 0;
    aiComboMask &= ~(1 << arg0->id);
    arg0->flags |= 0x80;
    for (i = 0; i < 16; i++) {
        if ((aiComboMask >> i) & 1) {
            arg0->comboPartners[k] = i;
            k++;
            btlUnits[i].comboLeader = arg0->id;
            if (k >= 3) {
                break;
            }
        }
    }
    return 1;
}
