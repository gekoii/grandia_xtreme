/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


void _sceMpegSetImageAddr(u32 addr) {
    _sceMpegCurMp->sys->imageAddr = addr;
}

s32 sceMpegDispWidth(sceMpeg *mp) {
    return mp->sys->displayWidth;
}

s32 sceMpegDispHeight(sceMpeg *mp) {
    return mp->sys->displayHeight;
}

/* Both return the address of frameCenterOffset as an int (as the library does). */
s32 sceMpegDispCenterOffX(sceMpeg *mp) {
    return (s32)mp->sys->frameCenterOffset;
}

s32 sceMpegDispCenterOffY(sceMpeg *mp) {
    return (s32)mp->sys->frameCenterOffset;
}

s32 _sceMpegSetBrokenLink(sceMpeg *mp, s32 brokenLink) {
    s32 old;
    SceMpegSys *sys;

    sys = mp->sys;
    old = sys->unkE8;
    sys->unkE8 = brokenLink;
    return old;
}
