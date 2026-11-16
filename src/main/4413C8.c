#include "common.h"
#include "main/n06.h"

void SndSeBankLoadStart(char *src, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;

    for (i = 0; i < 31; i++) {
        sndSeWork.name[i] = src[i];
        if (src[i] == 0) {
            break;
        }
    }
    sndSeWork.unk64 = arg3;
    sndSeWork.bankHandle = arg1;
    sndSeWork.bankIndex = arg2;
    sndSeWork.loadState = 0;
}
