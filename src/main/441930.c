#include "common.h"
#include "snd.h"

u8 SndSeGetSlotState(u8 arg0, s32 arg1) {
    return sndSeSlots[arg0].u.shifted[arg1].state;
}
