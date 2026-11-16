/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

typedef struct {
    s32 cmd;
    u8 pad[0x1C];
} Msg;

extern void _sceMpegDoCallback(sceMpeg *, Msg *);

void _sceMpegLoadDefaultQm(s32 arg0, s32 arg1) {
    Msg m;

    m.cmd = 2;
    _sceMpegDoCallback(_sceMpegCurMp, &m);
    _sceMpegIpuWait();
    *(volatile s32 *)0x10002000 = 0;
    _sceMpegIpuWait();
    *(volatile s32 *)0x1000B410 = arg1 & 0x0FFFFFFF;
    *(volatile s32 *)0x1000B420 = 4;
    *(volatile s32 *)0x1000B400 = 0x101;
    _sceMpegIpuCmd(arg0);
    _sceMpegIpuWait();
    m.cmd = 3;
    _sceMpegDoCallback(_sceMpegCurMp, &m);
}
