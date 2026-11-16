/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


static inline void ipu_wait(void) {
    s32 i = 0;

    while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000) {
        if (i++ > 5000) {
            _sceMpegCallNodata((s32)_sceMpegCurMp);
            i = 0;
        }
    }
}

s32 _sceMpegIpuVdec(s32 tbl) {
    s32 i = 0;
    s32 c;
    s64 r, top;
    u32 bp;

    ipu_wait();
    c = (tbl << 26) | 0x30000000;
    *(volatile s32 *)0x10002000 = c;
    _sceMpegBitsStale = _sceMpegIpuCmdInvalidatesBfr[c >> 28];
    while ((r = *(volatile s64 *)0x10002000) < 0) {
        if (i++ > 5000) {
            i = 0;
            _sceMpegCallNodata((s32)_sceMpegCurMp);
        }
    }
    bp = *(volatile u32 *)0x10002020;
    top = *(s64 *)0x10002030;
    _sceMpegBfr = top;
    if (top < 0) {
        _sceMpegIncnt = -(bp & 0x1F) & 0x1F;
    } else {
        _sceMpegIncnt = 32;
    }
    Fault_Flag = (s32)r == 0;
    return (s16)r;
}
