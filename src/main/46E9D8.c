/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libdma.h"
#include "protos/main.h"

extern char _sceDmaSyncTimeoutStr[];

s32 sceDmaWatch(volatile sceDmaChan *d, void *addr, s32 mode, s32 timeout) {
    if (mode == 1) {
        return d->madr < addr;
    }
    if (timeout == 0) {
        timeout = 0x1000000;
    }
    while (d->madr < addr) {
        if (--timeout < 0) {
            printf(_sceDmaSyncTimeoutStr);
            {
                tD_CHCR c = d->chcr, t;

                if (c.STR) {
                    do {
                        t = c;
                        t.STR = 0;
                        c = t;
                    } while (c.STR);
                    d->chcr = t;
                }
            }
        }
    }
    return 0;
}
