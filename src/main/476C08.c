/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


s64 _sceMpegIpuReadCmd(void) {
    s32 n;
    s64 v;

    n = 0;
    while ((v = *(volatile s64 *)0x10002000) < 0) {
        if (*(volatile s32 *)0x10002010 & 0x4000) {
            break;
        }
        if (n++ > 5000) {
            _sceMpegCallNodata((s32)_sceMpegCurMp);
            n = 0;
        }
    }
    return v;
}
