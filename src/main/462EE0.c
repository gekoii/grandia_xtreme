#include "common.h"
#include "main/n10.h"

s32 SndStrmIndexFind(s32 arg0, s32 arg1, s32 *arg2) {
    s32 i;
    s32 *p;
    s32 v;

    arg1 <<= 20;
    p = sndStrmIndex;
    for (i = 0; i < 0x400; i++) {
        v = *p++;
        if ((v & 0x7FF00000) == arg1) {
            *arg2 = (*p & 0xFFFFF) - (v & 0xFFFFF);
            return v;
        }
    }
    return -4;
}
