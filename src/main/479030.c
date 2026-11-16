/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 _sceMpegBsShowBits(u64 *arg0, s32 arg1) {
    return *arg0 >> (64 - arg1);
}
