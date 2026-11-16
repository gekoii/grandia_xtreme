/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s64 unk0;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s64 unk18;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} S;

void _sceMpegBsFlushBits(S *, s32);

void _sceMpegBsInit(S *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk8 = arg1;
    arg0->unkC = arg1;
    arg0->unk0 = 0;
    arg0->unk10 = 0;
    arg0->unk18 = 0;
    arg0->unk20 = arg2;
    arg0->unk24 = arg2 + arg3;
    arg0->unk28 = arg3;
    _sceMpegBsFlushBits(arg0, 0);
}
