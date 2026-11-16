/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 n0;
    s32 n1;
    u8 *dst0;
    u8 *dst1;
    u8 buf0[0x40];
    u8 buf1[0x40];
} Xfer;

void _sceMcReadFixAlign(s32 arg0) {
    Xfer *p;
    s32 i;
    u8 *d;

    p = (Xfer *)(arg0 | 0x20000000);
    if (p->n0 != 0) {
        d = p->dst0;
        for (i = 0; i < p->n0; i++) {
            *d++ = p->buf0[i];
        }
    }
    if (p->n1 != 0) {
        d = p->dst1;
        for (i = 0; i < p->n1; i++) {
            *d++ = p->buf1[i];
        }
    }
}
