#include "common.h"
#include "battle/misc.h"

s32 PtclBtlDustGroupFindFree(void) {
    s32 i;
    s32 *p;

    i = 0;
    p = ptclBtlDustGroups;
    if (*p != 0) {
        do {
            i++;
            p++;
        } while (i < 32 && *p != 0);
    }
    return i;
}
