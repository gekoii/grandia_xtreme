/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"

void sceMpegGetDecodeMode(sceMpeg *mp, s32 *ni, s32 *np, s32 *nb) {
    SceMpegSys *sys;

    sys = mp->sys;
    *ni = sys->decodeModeI;
    *np = sys->decodeModeP;
    *nb = sys->decodeModeB;
}

s32 sceMpegIsEnd(sceMpeg *mp) {
    return mp->sys->isEnd;
}

s32 sceMpegIsRefBuffEmpty(sceMpeg *mp) {
    return mp->sys->sequenceFramenum == 0;
}
