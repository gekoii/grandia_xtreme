#include "common.h"

s32 PtclSystemCalcSize(s32 arg0, s32 arg1, s32 arg2) {
    u32 s = 0x20;

    s += arg0 * 4;
    s = (s + 0xF) & 0xFFFFFFF0;
    s += arg1 * 0x1C;
    s = (s + 0xF) & 0xFFFFFFF0;
    s += arg2 * 0x10;
    s = (s + 0xF) & 0xFFFFFFF0;
    return s;
}
