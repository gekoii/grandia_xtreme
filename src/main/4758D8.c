/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

void _sceMpegSetIpuMp1(s32 arg0) {
    *(s32 *)0x10002010 = (*(s32 *)0x10002010 & 0xFF7FFFFF) | (arg0 << 0x17);
}
