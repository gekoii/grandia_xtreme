#include "common.h"
#include "snd.h"




s32 SndSeFindFreeSlot(u8 arg0) {
    s32 i;
    SndSeSlotShifted *e = sndSeSlots[arg0].u.shifted;

    for (i = 0; i < 0x40; i++, e++) {
        if (e->state == 0 && e->timer == 0) {
            return i;
        }
    }
    return -1;
}
