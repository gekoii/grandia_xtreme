/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"




void sceMpegReset(sceMpeg *arg0) {
    SceMpegSys *p;

    p = arg0->sys;
    p->isEnd = 0;
    p->sequenceFramenum = 0;
    p->outputState = 0;
    arg0->frameCount = 0;
    p->lastPts = -1;
    p->readyFramenum = 0;
    _sceMpegResetIpu();
    _sceMpegBitstreamFramenum = 0;
    _sceMpegResetMpeg2Flag();
}
