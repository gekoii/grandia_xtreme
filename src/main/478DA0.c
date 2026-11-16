/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"




extern s32 _sceMpegDoCallback(sceMpeg *, s32 *);
extern s32 AddDmacHandler(s32, void *, s32);
extern s32 RemoveDmacHandler(s32, s32);
extern s32 DisableDmac(s32);
extern s32 EnableDmac(s32);

void _sceMpegCsc(SceMpegPicture *arg0) {
    SceMpegSys *sub;
    s32 cmd[8];
    s32 id;
    s32 n;
    s32 c10;
    u32 x0;

    n = arg0->mbWidth * arg0->mbHeight;
    sub = *(SceMpegSys *volatile *)&_sceMpegCurMp->sys;
    *(volatile s32 *)cmd = 2;
    _sceMpegDoCallback(_sceMpegCurMp, cmd);
    if (*(volatile u32 *)0x10002010 & 0x4000) {
        *(volatile u32 *)0x10002010 = 0x40000000;
    }
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    _sceMpegIpuCmd(0);
    c10 = 0x10;
    x0 = arg0->addr;
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    _sceMpegIpuToQwcLeft = n * 0x18;
    _sceMpegIpuToSrcAddr = x0 & 0x0FFFFFFF;
    if (_sceMpegIpuToQwcLeft > 0xFFFF) {
        id = AddDmacHandler(4, _sceMpegCscToHandler, 0);
        *(volatile u32 *)0x1000E010 = c10;
        EnableDmac(4);
        *(volatile u32 *)0x1000B410 = _sceMpegIpuToSrcAddr;
        *(volatile u32 *)0x1000B420 = 0xFFFF;
        *(volatile u32 *)0x1000B400 = 0x101;
        _sceMpegIpuToSrcAddr = (_sceMpegIpuToSrcAddr + 0xFFFF0) & 0x0FFFFFFF;
        _sceMpegIpuToQwcLeft -= 0xFFFF;
        if (n < 0x400) {
            _sceMpegCscSingle(sub->imageAddr, n);
        } else {
            _sceMpegCscMulti(sub->imageAddr, n);
        }
        DisableDmac(4);
        RemoveDmacHandler(4, id);
    } else {
        *(volatile u32 *)0x1000B410 = _sceMpegIpuToSrcAddr;
        *(volatile u32 *)0x1000B420 = _sceMpegIpuToQwcLeft;
        *(volatile u32 *)0x1000B400 = 0x101;
        _sceMpegIpuToQwcLeft = 0;
        if (n < 0x400) {
            _sceMpegCscSingle(sub->imageAddr, n);
        } else {
            _sceMpegCscMulti(sub->imageAddr, n);
        }
    }
    cmd[0] = 3;
    _sceMpegDoCallback(_sceMpegCurMp, cmd);
}
