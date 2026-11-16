#include "common.h"
#include "snd.h"
#include "protos/main.h"

extern void MemSet(void *, s32, s32);

void SndSeStopGroup(u8 arg0) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        SndSeStop(arg0, i);
    }
    MemSet(&sndSeSlots[arg0], 0, 0xC10);
}
