#include "common.h"
#include "party.h"

s16 ItemMoveSlotIndex(s16 x) {
    s32 i;
    PtyCombo *e;

    if ((u16)(x - 101) < 47) {
        return (x - 101) / 6 * 5 + (x - 101) % 6;
    }
    e = ptyComboTable;
    for (i = 0; i < 50; i++) {
        if (e->moveId == x) {
            return i + 40;
        }
        e++;
    }
    return 89;
}
