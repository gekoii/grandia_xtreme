/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"

extern M2C_UNK _sceMpegErrorFmt;

void _sceMpegPrintError(s32 arg0) {
    printf(&_sceMpegErrorFmt, arg0);
}
