/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s64 x0;
    s32 x8;
    u32 xC;
    s32 x10;
    s32 x14;
    s64 x18;
    s32 x20;
    u32 x24;
    s32 x28;
} S479180;

extern void _sceMpegBsFlushBits(S479180 *, s32);

void _sceMpegBsSkipBytes(S479180 *arg0, s32 arg1) {
    arg0->x0 = 0;
    arg0->x10 = 0;
    arg0->x18 += arg1 * 8;
    arg0->xC = arg0->x8 + (s32)(arg0->x18 >> 3);
    if (arg0->xC >= arg0->x24) {
        arg0->xC -= arg0->x28;
    }
    _sceMpegBsFlushBits(arg0, 0);
}
