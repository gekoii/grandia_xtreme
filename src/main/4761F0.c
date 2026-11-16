/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"



typedef struct {
    s32 unk0;
    s32 unk4;
    s32 pad8[2];
    s32 unk10;
    s32 unk14;
} A;

extern s32 _sceMpegSkippedMbIPicStr;

s32 _skippedMacroblock(A *arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    s32 ret;

    ret = 1;
    _sceMpegDcReset = 1;
    _sceMpegMbSlots.slot[_sceMpegMbSlots.cur].skipped = 1;
    if (picture_coding_type == 2) {
        arg0->unk14 = 0;
        arg0->unk10 = 0;
        arg0->unk4 = 0;
        arg0->unk0 = 0;
    }
    if (picture_structure == 3) {
        *arg1 = 2;
    } else {
        *arg1 = 1;
        arg2[0] = arg2[1] = picture_structure == 2;
    }
    if (picture_coding_type == 1) {
        _sceMpegError(&_sceMpegSkippedMbIPicStr);
        ret = 0;
    }
    *arg3 &= ~1;
    return ret;
}
