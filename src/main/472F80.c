/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _sceMpegResetMpeg2Flag(void) {
    MPEG2_Flag = 0;
    _sceMpegSetIpuMp1(1);
}
