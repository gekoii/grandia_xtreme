/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void _sceMpegErrorf(s32 arg0, s32 arg1) {
    u8 buf[256];
    sprintf(buf, arg0, arg1);
    _sceMpegError(buf);
}
