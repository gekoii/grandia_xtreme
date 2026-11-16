/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


#define REG(a) (*(volatile u32 *)(a))

s32 _sceMpegCscToHandler(void) {
    REG(0x1000E010) = 0x10;
    _sceMpegIpuToIntrCount++;
    if (_sceMpegIpuToQwcLeft == 0) {
        return 1;
    }
    if (_sceMpegIpuToQwcLeft > 0xFFFF) {
        REG(0x1000B410) = _sceMpegIpuToSrcAddr;
        REG(0x1000B420) = 0xFFFF;
        REG(0x1000B400) = 0x101;
        _sceMpegIpuToSrcAddr = (_sceMpegIpuToSrcAddr + 0xFFFF0) & 0x0FFFFFFF;
        _sceMpegIpuToQwcLeft -= 0xFFFF;
    } else {
        REG(0x1000B410) = _sceMpegIpuToSrcAddr;
        REG(0x1000B420) = _sceMpegIpuToQwcLeft;
        REG(0x1000B400) = 0x101;
        _sceMpegIpuToQwcLeft = 0;
    }
    return 0;
}
