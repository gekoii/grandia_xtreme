#include "common.h"

typedef struct {
    u8 pad[0xB0];
    s32 unkB0;
    u8 pad2[0x24];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
} Sub;

typedef struct {
    u8 pad[0x40];
    Sub *sub;
} Obj;

s32 _sceMpegDecodeNextPicture(Obj *);

s32 sceMpegGetPicture(Obj *arg0, s32 arg1, s32 arg2) {
    Sub *s = arg0->sub;
    arg1 = (arg1 & 0x0FFFFFFF) | 0x20000000;
    s->unkB0 = 1;
    s->unkD8 = arg1;
    s->unkE4 = arg2;
    s->unkE0 = 0;
    s->unkDC = 0;
    return _sceMpegDecodeNextPicture(arg0);
}
