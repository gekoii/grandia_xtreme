/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0xB0];
    s32 b0;
    u8 pad2[0xD8 - 0xB4];
    s32 addr;
    s32 w;
    s32 h;
    s32 size;
} Ctx;

typedef struct {
    u8 pad[0x40];
    Ctx *ctx;
} Obj;

void _sceMpegDecodeNextPicture(Obj *, s32, s32, s32, s32);

void sceMpegGetPictureRAW8xy(Obj *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Ctx *c;
    s32 size = arg2 * arg3;
    arg2 *= 16;
    arg1 = (arg1 & 0x0FFFFFFF) | 0x20000000;
    c = arg0->ctx;
    arg3 *= 16;
    c->h = arg3;
    c->addr = arg1;
    c->size = size;
    c->w = arg2;
    do {
        c->b0 = 0;
        _sceMpegDecodeNextPicture(arg0, arg1, arg2, arg3, size);
    } while (0);
}
