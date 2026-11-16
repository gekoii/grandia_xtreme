/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void sceMpegAddBs(s32 arg0, s32 arg1, s32 arg2) {
    s32 size = (arg2 + 0x13) / 16 * 16;
    _sceMpegInputBuf = arg1;
    _sceMpegInputSize = size;
    _sceMpegSendBsChain(arg1, size);
}
