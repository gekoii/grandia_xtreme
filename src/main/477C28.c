/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"
#include "m2c_macros.h"

extern M2C_UNK _sceMpegOddFieldPicturesStr;
extern M2C_UNK _sceMpegPicStructureStr;

s32 _decodePictureData(s32 arg0, s32 arg1) {
    s32 temp_v0;
    SceMpegPicture *var_s0;

    if (picture_structure == 3) {
        if (Second_Field != 0) {
            _sceMpegError(&_sceMpegOddFieldPicturesStr);
            Second_Field = 0;
        }
    }
    switch (picture_structure) {                           /* irregular */
    case 3:
        var_s0 = _sceMpegCurFrame;
        break;
    case 1:
        var_s0 = _sceMpegCurTop;
        break;
    case 2:
        var_s0 = _sceMpegCurBottom;
        break;
    default:
        var_s0 = _sceMpegCurFrame;
        _sceMpegError(&_sceMpegPicStructureStr);
        break;
    }
    temp_v0 = _pictureData(arg0);
    if (temp_v0 != 0) {
        var_s0->ref = 1;
    }
    return temp_v0;
}
