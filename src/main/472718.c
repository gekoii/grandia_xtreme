/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad0[0xB0];
    s32 fB0;
    u8 padB4[0x24];
    u32 fD8;
    s32 fDC;
    s32 fE0;
    s32 fE4;
} C;

typedef struct {
    u8 pad[0x40];
    C *c;
} O;

s16 _sceMpegDecodeNextPicture(O *);

s32 sceMpegGetPictureRAW8(O *arg0, u32 arg1, s32 arg2) {
    C *c = arg0->c;
    c->fE4 = arg2;
    c->fD8 = (arg1 & 0x0FFFFFFF) | 0x20000000;
    c->fB0 = 0;
    c->fE0 = 0;
    c->fDC = 0;
    return _sceMpegDecodeNextPicture(arg0);
}
