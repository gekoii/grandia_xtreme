#include "common.h"
#include "snd.h"




s32 SndSeIsPlaying(u8 arg0, s32 arg1) {
    s32 i;
    SndSeSlotShifted *p = sndSeSlots[arg0].u.shifted;

    for (i = 0; i < 0x40; i++, p++) {
        if (p->state == 1 && p->index == arg1) {
            return 1;
        }
    }
    return 0;
}
