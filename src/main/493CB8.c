/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern char _sceSifRebootUdnlStr[];
extern char _sceSifRebootTooLongFmt[];

s32 sceSifRebootIop(char *arg0) {
    char buf[0x50];
    char *s;
    char *src;
    u8 *d;

    src = _sceSifRebootUdnlStr;
    for (s = arg0; *s != 0; s++) {
    }
    if ((u32)(s + 11 - arg0) > 0x50) {
        printf(_sceSifRebootTooLongFmt, arg0);
        return 0;
    }
    sceSifInitRpc(0);
    sceSifExitRpc();
    d = buf;
    while (*src != 0) {
        *d++ = *src++;
    }
    while (*arg0 != 0) {
        *d++ = *arg0++;
    }
    *d = 0;
    return sceSifResetIop(buf, 0);
}
