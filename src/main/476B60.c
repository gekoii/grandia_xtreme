/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _sceMpegIpuWait(void) {
    s32 i = 0;

    while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000) {
        if (i++ > 5000) {
            _sceMpegCallNodata((s32)_sceMpegCurMp);
            i = 0;
        }
    }
}
