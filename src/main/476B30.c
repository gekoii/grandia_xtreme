/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


void _sceMpegIpuCmd(u32 arg0) {
    *(volatile u32 *)0x10002000 = arg0;
    _sceMpegBitsStale = _sceMpegIpuCmdInvalidatesBfr[arg0 >> 28];
}
