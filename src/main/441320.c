#include "common.h"
#include "main/n06.h"
#include "protos/main.h"

void MemSet(void *, s32, s32);
extern s32 sndSeUnusedFlag;

void SndSeInit(void) {
    MemSet(sndSeSlots, 0, 0x3040);
    MemSet(&sndSeWork, 0, 0x88);
    sndSeRequestEnabled = 1;
    sndSeWork.loadState = -1;
    sndSeUnusedFlag = 0;
    SndSeResume();
}
