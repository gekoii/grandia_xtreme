#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u32 unkC;
    s64 unk10;
    s64 unk18;
} Req;

extern u8 movStrPtsOverflow[];


s32 videoCallback(s32 arg0, Req *arg1, MovReadBuf *arg2) {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    s32 off;
    u32 n;
    u32 rest;
    u32 left;
    s32 r;

    off = arg1->unk8;
    rest = arg1->unkC;
    n = (u32)(arg2->data + arg2->size) - off;
    if (rest < n) {
        n = rest;
    }
    left = rest - n;
    videoDecBeginPut(&movVideoDec, &a, &b, &c, &d);
    r = copy2area((a & 0x0FFFFFFF) | 0x20000000, b, (c & 0x0FFFFFFF) | 0x20000000, d, off, n, arg2, left);
    if (r > 0) {
        if (videoDecPutTs(&movVideoDec, arg1->unk10, arg1->unk18, a, r) == 0) {
            MovErrMessage(movStrPtsOverflow);
        }
    }
    videoDecEndPut(&movVideoDec, r);
    return r > 0;
}
