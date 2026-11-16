/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"




void _sceMpegSetPictureReady(void) {
    SceMpegSys *p;
    s32 t;

    p = _sceMpegCurMp->sys;
    if (p->outputState != 2) {
        t = _sceMpegBitstreamFramenum;
        p->outputState = 2;
        p->readyFramenum = t;
    }
    _sceMpegPictureDone = 1;
}
