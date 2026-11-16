#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemCharUnequipOwned(s16 a, s16 mode, s16 id) {
    s32 n;
    s32 cnt;
    s32 i;
    s32 j;

    n = ptyCharDefTable[a].eggSlots;
    i = 0;
    cnt = 6 - n;
    switch (mode) {
    case 0:
        for (;;) {
            if (i >= n) break;
            if (ptyCharData[a].slots[i] == id) {
                ItemCharUnequipEgg(a + 1000, i);
                break;
            }
            i++;
        }
        return -1;
    case 1:
        for (; i < cnt; i++) {
            j = i + n;
            if (ptyCharData[a].slots[j] == id) {
                ItemCharUnequipBookSkills(a, j);
                ItemCharUnequipBook(a + 1000, j);
                return -1;
            }
        }
        return -1;
    }
    return 0;
}
