#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndIopCmdPushString(s8 *arg0) {
    s32 n;
    u8 *d;

    n = 0;
    d = (u8 *)&sndIopCmdBufPtr[24]; /* string area after the 24 commands */
    if (sndIopStringCmdPending != 0) {
        return -1;
    }
    while (*arg0 != 0) {
        *d = *arg0;
        n++;
        arg0++;
        d++;
        if (n >= 0x7F) {
            goto out; /* not break: gcc would fold it into the loop test */
        }
    }
out:
    *d = 0;
    if (SndIopCmdPush(0x80, 0x180, d, n) == 0) {
        return -2;
    }
    sndIopStringCmdPending = 1;
    return 0;
}
