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

u32 _showBits(s32 n) {
    if (_sceMpegBitsStale != 0 || _sceMpegIncnt < n) {
        ipu_wait();
        *(volatile u32 *)0x10002000 = 0x40000000;
        _sceMpegBitsStale = _sceMpegIpuCmdInvalidatesBfr[4];
        _sceMpegBfr = _sceMpegIpuReadCmd();
        _sceMpegIncnt = 32;
    }
    return _sceMpegBfr >> (32 - n);
}
