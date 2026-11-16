/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "m2c_macros.h"

extern M2C_UNK _sceMpegSecondFieldMissingStr;

void _outputLastFrameOfSequence(s32 arg0) {
    if (Second_Field != 0) {
        _sceMpegError((s32) &_sceMpegSecondFieldMissingStr);
    } else if (picture_structure == 3) {
        _writeFrame(_sceMpegBwdRefFrame, arg0 - 1);
    } else {
        _writeFieldPair(_sceMpegBwdRefTop, _sceMpegBwdRefBottom, arg0 - 1);
    }
    Second_Field = 0;
}
