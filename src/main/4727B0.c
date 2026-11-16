/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0x94];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
} Inner;

typedef struct {
    u8 pad[0x40];
    Inner *unk40;
} Outer;

void sceMpegSetDecodeMode(Outer *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Inner *p = arg0->unk40;
    p->unk94 = arg1;
    p->unk98 = arg2;
    p->unk9C = arg3;
}
