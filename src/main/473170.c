/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"



typedef struct {
    s32 a;
    s32 b;
} Arg;

void _sceMpegDoCallback(sceMpeg *, Arg *);

void _sceMpegError(s32 arg0) {
    Arg s;
    if (_sceMpegCurMp != NULL && _sceMpegCurMp->sys != NULL && _sceMpegCurMp->sys->callbacks[0].func != 0) {
        s.b = arg0;
        s.a = 0;
        _sceMpegDoCallback(_sceMpegCurMp, &s);
    } else {
        _sceMpegPrintError(arg0);
    }
}
