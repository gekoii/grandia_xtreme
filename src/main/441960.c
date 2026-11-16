#include "common.h"
#include "snd.h"

void SndSeClearHold(u8 arg0, s32 arg1) {
    SndSeSlotShifted *it = (SndSeSlotShifted *)((u8 *)sndSeSlots + (arg1 * 0x30 + arg0 * 0xC10));
    if (it->flags & 0x10) {
        it->flags ^= 0x10;
        if (it->state == 4) it->state = 0;
    }
}
