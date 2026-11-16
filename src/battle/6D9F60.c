#include "common.h"
#include "battle/unit.h"

void BtlUnitSetReviveHpMode(s32 *p, s8 v) {
    s32 i;
    if (p == NULL) {
        for (i = 0; i < 16; i++) {
            btlReviveHpMode[i] = v;
        }
    } else {
        btlReviveHpMode[*p] = v;
    }
}
