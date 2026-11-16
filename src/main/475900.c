/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

#define REG(a) (*(volatile s32 *)(a))

void _sceMpegDoCallback(sceMpeg *, s32 *);
s32 DIntr(void);
s32 EIntr(void);
extern char _sceMpegBdecErrorStr[];

s32 _sceMpegCheckBdec(void) {
    s32 a[8];
    s32 b[8];
    s32 ret;
    s64 top;
    s32 bp;
    s32 raw;

    ret = 1;
    _sceMpegIpuWait();
    while (REG(0x1000B020) != 0 && !(REG(0x10002010) & 0x4000)) {
        if (REG(0x1000B420) == 0 && !(REG(0x1000B400) & 0x100)) {
            a[0] = 1;
            _sceMpegDoCallback(_sceMpegCurMp, a);
        }
    }
    raw = REG(0x10002020);
    top = *(s64 *)0x10002030;
    _sceMpegBfr = top;
    if (top < 0) {
        bp = raw & 0x1F;
        _sceMpegIncnt = bp != 0 ? 32 - bp : 0;
    } else {
        _sceMpegIncnt = 32;
    }
    if (REG(0x10002010) & 0x4000) {
        ret = 0;
        _sceMpegError(_sceMpegBdecErrorStr);
        b[0] = 2;
        _sceMpegDoCallback(_sceMpegCurMp, b);
        REG(0x10002010) = 0x40000000;
        b[0] = 3;
        _sceMpegDoCallback(_sceMpegCurMp, b);
        DIntr();
        REG(0x1000F590) = REG(0x1000F520) | 0x10000;
        REG(0x1000B000) = 0;
        REG(0x1000F590) = REG(0x1000F520) & ~0x10000;
        EIntr();
        REG(0x1000B020) = 0;
    }
    return ret;
}
