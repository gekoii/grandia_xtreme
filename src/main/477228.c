/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void _nextStartCode(void) {
    s32 n;

    _sceMpegIpuWait();
    n = -(*(volatile s32 *)0x10002020 & 7) & 7;
    if (n != 0) {
        _flushBuffer(n);
    }
    while (_showBits(0x18) != 1) {
        _flushBuffer(8);
    }
}
