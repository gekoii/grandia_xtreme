#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

extern s8 sndMovieStreaming;
void SndPcmMonitorOn(void) {
    s32 i;
    s64 *p;

    sndPcmMonitorOff = 0;
    SndIopCmdPush(0x2C0, 1, 0, 0);
    p = (s64 *)sndPcmBuf;
    for (i = 0; i < 0x8000; i += 8) {
        *p++ = 0;
    }
    sndMovieStreaming = 0;
}
