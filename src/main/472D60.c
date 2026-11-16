/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"

s32 _decodeFramePicture(void);
s32 _decodeFieldPictures(s32, s32, s32);

s32 _decodePicture(s32 arg0, s32 arg1, s32 arg2) {
    if (picture_structure != 3) {
        return _decodeFieldPictures(arg0, arg1, arg2);
    }
    return _decodeFramePicture();
}
