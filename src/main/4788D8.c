/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

void _sceMpegDoCallback(sceMpeg *, s32 *);

void _sceMpegCscSingle(u32 addr, s32 qwc) {
    s32 cmd[8];
    s32 n;

    n = qwc << 6;
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    *(volatile u32 *)0x1000B010 = addr & 0x0FFFFFFF;
    *(volatile u32 *)0x1000B020 = n;
    *(volatile u32 *)0x1000B000 = 0x100;
    _sceMpegIpuCmd(qwc | 0x70000000);
    cmd[0] = 4;
    _sceMpegDoCallback(_sceMpegCurMp, cmd);
    while ((*(volatile u32 *)0x1000B000 >> 8) & 1) {
    }
    while (*(volatile s32 *)0x10002010 < 0) {
    }
}
