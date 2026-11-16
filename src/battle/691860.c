#include "common.h"
#include "battle/misc.h"

s8 BtlCmdListSpellEggCount(s32 id) {
    BtlCmdList *b = &btlCmdList;
    s32 i;
    s8 cnt = 0;
    s32 k;
    BtlCmdListCell *s;
    i = 0;
    if (b->count > i) {
        do {
            if (b->rowState[i] >= 0) {
                s = &b->cells[i * 6];
                for (k = 0; k < 6; k++, s++) {
                    if (s->flags != 0 && !(s->flags & 2) && id == s->id) {
                        cnt++;
                        break;
                    }
                }
            }
            i++;
        } while (i < b->count);
    }
    return cnt;
}
