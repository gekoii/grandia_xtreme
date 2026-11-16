#include "common.h"
#include "party.h"

s16 BtlSpecialMoveRecordIndex(s16 id) {
    s32 v;
    s32 i;
    PtyCombo *e;

    v = id - 0x65;
    if ((u16)v < 0x2F) {
        return v / 6 * 5 + v % 6;
    }
    e = ptyComboTable;
    for (i = 0; i < 50; i++, e++) {
        if (e->moveId == id) {
            return i + 0x28;
        }
    }
    return 0x59;
}
