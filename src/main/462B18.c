#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

void SndPcmMonitorOff(void) {
    s32 i;
    s64 *p;

    p = (s64 *)sndPcmBuf;
    for (i = 0; i < 0x8000; i += 8) {
        *p++ = 0;
    }
    SndIopCmdPush(0x2C0, 0, 0, 0);
    sndPcmMonitorOff = 1;
}
