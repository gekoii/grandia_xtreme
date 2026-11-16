/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"




void _frameReorder(s32 arg0, s32 arg1) {
    SceMpegSys *b;
    SceMpegPicture *x;
    SceMpegPicture *y;

    b = _sceMpegCurMp->sys;
    if (arg1 != 0) {
        if (picture_structure == 3) {
            if (picture_coding_type == 3) {
                x = _sceMpegAuxFrame;
            } else {
                x = _sceMpegFwdRefFrame;
            }
            ((void (*)(SceMpegPicture *, s32))_writeFrame)(x, arg0 - 1);
        } else {
            if (picture_coding_type == 3) {
                x = _sceMpegAuxTop;
                y = _sceMpegAuxBottom;
            } else {
                x = _sceMpegFwdRefTop;
                y = _sceMpegFwdRefBottom;
            }
            ((void (*)(SceMpegPicture *, SceMpegPicture *, s32))_writeFieldPair)(x, y, arg0 - 1);
        }
    }
    if (b->nextPtsState == 1) {
        b->nextPtsState = 2;
    }
}
