/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern void _sceMpegBsFlushBits(s32, s32);

s32 _sceMpegBsMarkerBit(s32 arg0) {
    s32 r;

    r = _sceMpegBsShowBits(arg0, 1);
    _sceMpegBsFlushBits(arg0, 1);
    return r;
}
