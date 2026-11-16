/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"





void _sceMpegGetTimeStamp(SceMpegPicture *arg0, s64 *arg1, s64 *arg2, s64 *arg3) {
    SceMpegSys *s;
    s32 s6;
    s32 s0;
    s64 s2;
    s32 s7;
    s32 s1;
    s64 t;

    s = _sceMpegCurMp->sys;
    if (s->ptsGapSet != 0 && arg0->pts < 0 && (s7 = s->lastPts) >= 0) {
        s0 = s->fieldCount;
        s2 = s0 & 1;
        t = __muldi3(s2, s->ptsGap & 1);
        s6 = s->ptsInterpCount;
        s1 = __muldi3(t, s6 & 1);
        *arg1 = s7 + ((s32)(__muldi3(s->ptsGap, s0) >> 1) + s1);
        if (__muldi3(s2, s->ptsGap & 1) != 0) {
            s->ptsInterpCount = s6 + 1;
        }
    } else {
        *arg1 = arg0->pts;
    }
    if (s->nextPtsState == 2 && s->nextPts >= 0) {
        *arg1 = s->nextPts;
        s->nextPtsState = 0;
        s->nextPts = -1;
    }
    *arg2 = arg0->dts;
    *arg3 = ((s64)arg0->progressiveSequence << 8) | ((s64)arg0->progressiveFrame << 7) | ((s64)arg0->topFieldFirst << 6) | ((s64)arg0->repeatFirstField << 5) | ((s64)arg0->pictureStructure << 3) | (s64)arg0->pictureCodingType;
}
