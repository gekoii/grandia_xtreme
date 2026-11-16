/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"




extern char _sceMpegBufTooSmallFmt[];

s32 _sceMpegCheckImageBuf(SceMpegPicture *arg0) {
    char buf[0x100];
    SceMpegSys *m;
    s32 ok;

    m = _sceMpegCurMp->sys;
    if (m->imageHeight != 0) {
        ok = m->imageWidth >= arg0->width && m->imageHeight >= arg0->height;
    } else {
        ok = m->imageSize >= arg0->mbWidth * arg0->mbHeight;
    }
    if (!ok) {
        sprintf(buf, _sceMpegBufTooSmallFmt, arg0->width, arg0->height);
        _sceMpegError(buf);
    }
    return ok;
}
