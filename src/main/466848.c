#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 WaitSema(s32);
void SignalSema(s32);

#define REG(a) (*(volatile u32 *)(a))

s32 viBufAddDMA(ViBuf *arg0) {
    s32 kick;
    s32 n;
    s32 i;
    s32 j;
    s32 mode;
    s32 idx;
    s32 done;
    s32 tail;
    s32 last;
    u32 madr;
    s32 cnt;
    s32 pos;
    s32 bytes;

    kick = 0;
    WaitSema(arg0->sema);
    if (arg0->isActive == 0) {
        return 0;
    }
    REG(0x1000F590) = 0x10000;
    setD4_CHCR(5);
    REG(0x1000F590) = 0;
    __asm__ volatile("sync");
    madr = REG(0x1000B400);
    pos = getFIFOindex(arg0, REG(0x1000B410));
    cnt = arg0->n;
    done = (pos + cnt - arg0->dmaStart) % cnt;
    arg0->dmaStart = (arg0->dmaStart + done) % cnt;
    arg0->dmaN -= done;
    tail = arg0->dmaStart + arg0->dmaN;
    idx = tail % cnt;
    bytes = arg0->readBytes;
    n = bytes / 2048;
    arg0->readBytes = bytes - n * 2048;
    if (n > 0) {
        last = (tail - 1 + cnt) % cnt;
        scTag2(&arg0->tag[last], arg0->data + last * 2048, 3, 0x80);
        kick = 1;
    }
    j = idx;
    for (i = 0; i < n; i++) {
        mode = (i == n - 1) ? 0 : 3;
        scTag2(&arg0->tag[j], arg0->data + j * 2048, mode, 0x80);
        j = (j + 1) % arg0->n;
    }
    arg0->dmaN += n;
    if (arg0->dmaN != 0) {
        if (kick) {
            madr = (madr & 0x0FFFFFFF) | 0x30000000;
        }
        setD4_CHCR(madr | 0x100);
    }
    SignalSema(arg0->sema);
    return 1;
}
