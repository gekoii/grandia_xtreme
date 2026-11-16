#include "common.h"

extern u16 fldBtlGbxKinds[];

s32 FldBattleFindEnemyGbx(u16 id) {
    s32 i;

    for (i = 0; i < 0x2F; i++) {
        if (id == fldBtlGbxKinds[i]) {
            return i * 4;
        }
    }
    return -1;
}
