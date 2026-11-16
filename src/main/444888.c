#include "common.h"
#include "protos/main.h"

typedef struct {
    u64 data[14];
} GsStoreImage;

s32 sceGsSetDefStoreImage(GsStoreImage *si, s16 sbp, s16 sbw, s16 psm, s16 x, s16 y, s16 w, s16 h);
void FlushCache(s32);
s32 sceGsExecStoreImage(GsStoreImage *si, u32 *dst);

void GphFbStoreImage(s32 x, s32 y, u32 w, u32 hh, u32 *dst) {
    GsStoreImage si;
    u32 n;
    s32 done;
    s32 off;
    s32 r;
    u32 h;
    s32 cy;
    s32 t;

    off = x & 0x3F;
    w += off;
    r = w & 0x3F;
    t = x - off;
    if (off == 0) {
        t = x;
    }
    x = t;
    cy = y;
    done = 0;
    if (r != 0) {
        w += 0x40 - r;
    }
    h = hh;
    do {
        if (w * h > 0x23000) {
            n = 0x23000 / w;
            h -= n;
        } else {
            n = h;
            done = 1;
        }
        sceGsSetDefStoreImage(&si, 0, 10, 0, x, cy, w, n);
        cy += n;
        FlushCache(0);
        sceGsExecStoreImage(&si, dst);
        sceGsSyncPath(0, 0);
        dst += w * n;
    } while (done == 0);
}
