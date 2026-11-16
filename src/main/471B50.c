/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s16 sceMpegDemuxPssRing(s32, s32, s32, s32, s32);

s32 sceMpegDemuxPss(s32 arg0, s32 arg1, s32 arg2) {
    return sceMpegDemuxPssRing(arg0, arg1, arg2, 0, -1);
}
