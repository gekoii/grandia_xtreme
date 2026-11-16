/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"

void sceIpuRestartDMA(s32);

void _sceMpegDefaultRestartDMA(u8 *arg0) {
    sceIpuRestartDMA(*(s32 *)(arg0 + 0x40) + 0x4C);
}
