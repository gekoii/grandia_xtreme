/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"





void _writeFieldPair(SceMpegPicture *a, SceMpegPicture *b) {
    SceMpegSys *s;
    SceMpegPicture *first, *second;
    s32 flag = 0;

    s = _sceMpegCurMp->sys;
    if (picture_structure == 2) {
        first = a;
        second = b;
        flag = 0x40;
    } else {
        first = b;
        second = a;
    }
    _sceMpegGetTimeStamp(first, &_sceMpegCurMp->pts, &_sceMpegCurMp->dts, &_sceMpegCurMp->flags);
    s->lastPts = _sceMpegCurMp->pts;
    s->fieldCount = 1;
    _sceMpegGetTimeStamp(second, &_sceMpegCurMp->pts2nd, &_sceMpegCurMp->dts2nd, &_sceMpegCurMp->flags2nd);
    s->lastPts = _sceMpegCurMp->pts2nd;
    s->fieldCount = 1;
    s->displayWidth = first->displayHorizontalSize;
    _sceMpegCurMp->flags |= flag;
    _sceMpegCurMp->flags2nd |= flag;
    s->displayHeight = first->displayVerticalSize;
    s->frameCenterOffset[0] = first->frameCenterOffset[0];
    s->frameCenterOffset[1] = second->frameCenterOffset[1];
    s->frameCenterOffset[3] = first->frameCenterOffset[3];
    s->frameCenterOffset[4] = second->frameCenterOffset[4];
    if (_sceMpegCheckImageBuf(a) == 0) {
        return;
    }
    if (a->ref != 1) {
        return;
    }
    if (b->ref != 1) {
        return;
    }
    a->mbHeight *= 2;
    if (s->outputCsc != 0) {
        _sceMpegCsc(a);
    } else {
        _sceMpegCopyRaw8(a);
    }
    a->mbHeight >>= 1;
    _sceMpegSetPictureReady();
}
