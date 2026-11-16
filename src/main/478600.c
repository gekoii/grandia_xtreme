/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"





void _writeFrame(SceMpegPicture *a) {
    SceMpegSys *s;

    s = _sceMpegCurMp->sys;
    _sceMpegGetTimeStamp(a, &_sceMpegCurMp->pts, &_sceMpegCurMp->dts, &_sceMpegCurMp->flags);
    s->lastPts = _sceMpegCurMp->pts;
    s->fieldCount = _sceMpegFieldCountTable[(s32)(_sceMpegCurMp->flags >> 5) & 0xF];
    s->displayWidth = a->displayHorizontalSize;
    s->displayHeight = a->displayVerticalSize;
    s->frameCenterOffset[0] = a->frameCenterOffset[0];
    s->frameCenterOffset[1] = a->frameCenterOffset[1];
    s->frameCenterOffset[2] = a->frameCenterOffset[2];
    s->frameCenterOffset[3] = a->frameCenterOffset[3];
    s->frameCenterOffset[4] = a->frameCenterOffset[4];
    s->frameCenterOffset[5] = a->frameCenterOffset[5];
    if (_sceMpegCheckImageBuf(a) == 0) {
        return;
    }
    if (a->ref != 1) {
        return;
    }
    if (s->outputCsc != 0) {
        _sceMpegCsc(a);
    } else {
        _sceMpegCopyRaw8(a);
    }
    _sceMpegSetPictureReady();
}
