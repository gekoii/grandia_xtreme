/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
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

void _flushBuffer(u32 cmd) {
    u32 c;

    ipu_wait();
    c = cmd | 0x40000000;
    *(volatile u32 *)0x10002000 = c;
    _sceMpegBitsStale = _sceMpegIpuCmdInvalidatesBfr[c >> 28];
    _sceMpegBfr = _sceMpegIpuReadCmd();
    _sceMpegIncnt = 32;
}
