#include "common.h"
#include "main/n06.h"

extern s32 sndSeLoadState; /* sndSeWork.loadState */

s32 SndSeIsActive(void) {
    return sndSeActive;
}

void SndSeResume(void) {
    sndSeActive = 1;
}

void SndSeSuspend(void) {
    sndSeActive = 0;
}

s32 SndSeBankLoadGetState(void) {
    return sndSeLoadState;
}
