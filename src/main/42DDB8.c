#include "common.h"
#include "pad.h"
#include "protos/main.h"

/* scePadRead data. */
typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    s8 s[4];
    u8 data[12];
} PadRaw;

extern PadRaw padReadBuf;

static inline void conv(u8 *dst, PadRaw *p, s32 i) {
    if (p->s[i] < 0) {
        *dst = -(u8)p->s[i];
    } else {
        *dst = ~(u8)p->s[i];
    }
}

u32 PadReadPort(s32 a0, s32 a1, Pad *out) {
    u32 ret = 0;
    s32 i;

    if (scePadRead(a0, a1, &padReadBuf) == 0) {
        return 0;
    }
    if (padReadBuf.b0 == 0) {
        out->held = ~((padReadBuf.b2 << 8) | padReadBuf.b3);
        conv(&out->rightX, &padReadBuf, 0);
        conv(&out->rightY, &padReadBuf, 1);
        conv(&out->leftX, &padReadBuf, 2);
        conv(&out->leftY, &padReadBuf, 3);
        if (out->pressureMode != 0) {
            PadRaw *q;
            u8 *src;

            for (i = 0; i < 12; i++) {
                q = &padReadBuf;
                src = &q->data[i];
                out->pressure[i] = *src;
            }
        } else {
            u8 *dst = &out->pressure[11];
            for (i = 11; i >= 0; i--) {
                *dst-- = 0;
            }
        }
        ret = padReadBuf.b1 >> 4;
    }
    return ret;
}
