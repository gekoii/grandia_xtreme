#include "common.h"
#include "party.h"
#include "protos/field.h"

s16 ItemCharUnequipAllSkills(u16 a) {
    s32 ret;
    s32 i;
    s32 n;
    s32 cnt;

    ret = 0;
    n = ptyCharDefTable[a].eggSlots;
    cnt = 6 - n;
    for (i = 0; i < cnt; i++) {
        if (ItemCharUnequipBookSkills(a, n + i) == 0) {
            ret = 1;
        }
    }
    return ret;
}
