#include "common.h"
#include "ptcl.h"

s32 FldSpriteFindFree(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = &ptclFldSlots->used;
    do {
        if (*p == 0) {
            return i;
        }
        i++;
        p += sizeof(PtclFldSprite);
    } while (i < 0x40);
    return -1;
}
