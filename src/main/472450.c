/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"



void sceMpegReset(sceMpeg *);
s32 sceMpegClearRefBuff(sceMpeg *);
void _sceMpegHeapInit(void *, s32, s32);
void _sceMpegHeapMark(void *);
s32 _sceMpegHeapAlloc(void *, s32, s32);
void _sceMpegInitMbBuf(void);
void _sceMpegError(void *);
extern char _sceMpegWorkAreaTooSmallStr[];
void _sceMpegDefaultStopDMA(void);
void _sceMpegDefaultRestartDMA(void);

s32 sceMpegCreate(sceMpeg *arg0, u32 arg1, u32 arg2) {
    SceMpegSys *s;
    u8 *heap;

    s = (SceMpegSys *)(((arg1 + 3) >> 2) * 4);
    arg2 -= (u32)s - arg1;
    if (arg2 < 0x118) {
        _sceMpegError(_sceMpegWorkAreaTooSmallStr);
        return 0;
    }
    heap = (u8 *)s + 0x108;
    arg0->sys = s;
    _sceMpegHeapInit(heap, (s32)s + 0x118, arg2 - 0x118);
    arg0->width = 0;
    arg0->height = 0;
    arg0->frameCount = 0;
    arg0->pts = -1;
    arg0->dts = -1;
    arg0->flags = 0;
    arg0->pts2nd = -1;
    arg0->dts2nd = -1;
    arg0->flags2nd = 0;
    s->frameCenterOffset[0] = 0;
    s->frameCenterOffset[1] = 0;
    s->frameCenterOffset[2] = 0;
    s->frameCenterOffset[3] = 0;
    s->frameCenterOffset[4] = 0;
    s->frameCenterOffset[5] = 0;
    s->displayWidth = 0;
    s->displayHeight = 0;
    s->firstFieldStructure = 0;
    s->imageAddr = 0;
    s->imageWidth = 0;
    s->imageHeight = 0;
    s->imageSize = 0;
    s->unkE8 = 0;
    s->nextPtsState = 0;
    s->callbacks[0].func = 0;
    s->callbacks[1].func = 0;
    s->callbacks[4].func = 0;
    s->callbacks[5].func = 0;
    s->callbacks[6].func = 0;
    s->nextPts = -1;
    s->callbacks[2].func = (void *) _sceMpegDefaultStopDMA;
    s->callbacks[3].func = (void *) _sceMpegDefaultRestartDMA;
    s->strCallbacks = (SceMpegStrCallback *) _sceMpegHeapAlloc(heap, 0x600, 8);
    s->strCallbackCount = 0;
    s->frameBuf[0] = 0;
    s->frameBuf[1] = 0;
    s->frameBuf[2] = 0;
    s->ptsGapSet = 0;
    s->ptsGap = 0;
    s->fieldCount = 0;
    s->ptsInterpCount = 0;
    s->readyFramenum = 0;
    s->outputCsc = 1;
    _sceMpegCurMp = arg0;
    s->lastPts = -1;
    s->decodeModeI = -1;
    s->decodeModeP = -1;
    s->decodeModeB = -1;
    _sceMpegInitMbBuf();
    sceMpegReset(arg0);
    sceMpegClearRefBuff(arg0);
    _sceMpegFwdRefFrame = &_sceMpegPicFrame0;
    _sceMpegBwdRefFrame = &_sceMpegPicFrame1;
    _sceMpegAuxFrame = &_sceMpegPicFrame2;
    _sceMpegFwdRefTop = &_sceMpegPicTop0;
    _sceMpegBwdRefTop = &_sceMpegPicTop1;
    _sceMpegAuxTop = &_sceMpegPicTop2;
    _sceMpegFwdRefBottom = &_sceMpegPicBottom0;
    _sceMpegBwdRefBottom = &_sceMpegPicBottom1;
    _sceMpegAuxBottom = &_sceMpegPicBottom2;
    _sceMpegHeapMark(heap);
}
