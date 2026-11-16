/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

typedef struct {
    u8 pad[4];
    s32 unk4;
    s32 unk8;
    u8 pad2[0xAC - 0xC];
    s32 unkAC;
} Sub;

typedef struct {
    u8 pad[8];
    s32 unk8;
    u8 pad2[0x40 - 0xC];
    Sub *unk40;
} S;


s32 _sceMpegOutputLastFrame(S *arg0) {
    Sub *sub = arg0->unk40;
    s32 ret = 0;

    if (sub->unk4 != 0 && sub->unk8 != 0) {
        _outputLastFrameOfSequence(_sceMpegBitstreamFramenum);
        ret = 1;
        arg0->unk8 = _sceMpegBitstreamFramenum - sub->unkAC;
        sub->unk4 = 0;
    }
    return ret;
}
