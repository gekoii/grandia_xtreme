/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libdma.h"
#include "protos/main.h"

/* Local view of sceDmaChan: only chcr volatile (sceDmaChan or a volatile
 * sceDmaChan * does not match). */
typedef struct {
    volatile tD_CHCR chcr;
    u32 p0[3];
    void *madr;
    u32 p1[3];
    u32 qwc;
    u32 p2[3];
    void *tadr;
} DmaChan;

extern char _sceDmaSyncTimeoutStr[];

void sceDmaSendN(DmaChan *d, void *addr, u32 size) {
    s32 timeout = 0x1000000;
    tD_CHCR w;

    while (w = d->chcr, w.STR) {
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
    if (d->madr != (void *)-1) {
        d->madr = addr;
    }
    d->qwc = size;
    {
        tD_CHCR c = d->chcr;
        c.MOD = 0;
        c.DIR = 1;
        c.STR = 1;
        d->chcr = c;
    }
}
