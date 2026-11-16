#include "common.h"
#include "main/n11.h"

void WaitSema(s32);
void SignalSema(s32);

#define REG(a) (*(volatile u32 *)(a))

s32 viBufGetTs(ViBuf *arg0, MovPts *out) {
    s32 found;
    s32 skipped;
    s32 cur;
    u32 dma;
    s32 period;
    s32 x;
    s32 count;
    s32 i;
    MovTimeStamp *e;
    s32 base;

    found = 0;
    skipped = 0;
    cur = REG(0x1000B410);
    dma = REG(0x10002020);
    x = arg0->env[7] & 0x7F;
    period = arg0->n << 11;
    cur -= (((dma >> 16) & 3) + ((dma >> 8) & 0xF)) * 16;
    WaitSema(arg0->sema);
    out->pts = -1;
    out->dts = -1;
    cur += x >> 3;
    cur += period;
    cur -= (s32)arg0->data;
    cur = (u32)cur % (u32)period;
    count = arg0->countTs;
    base = arg0->wtTs;
    if (count > 0) {
        for (i = 0; i < count && !found; i++) {
            e = &arg0->ts[(base - count + arg0->nTs + i) % arg0->nTs];
            if ((cur + period - e->pos) % period < e->len) {
                if (e->pts >= 0) {
                    out->pts = e->pts;
                    out->dts = e->dts;
                    e->pts = -1;
                    e->dts = -1;
                }
                found = 1;
            } else {
                skipped++;
            }
        }
        arg0->countTs = count - (count < skipped ? count : skipped);
    }
    SignalSema(arg0->sema);
    return 1;
}
