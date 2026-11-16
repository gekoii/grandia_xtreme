/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"

void _sceMpegDoCallback(s32 a0, s32 *a1);

void _sceMpegCallNodata(s32 a0) {
    s32 buf[8];
    buf[0] = 1;
    _sceMpegDoCallback(a0, buf);
}
