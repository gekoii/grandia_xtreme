#include "common.h"
#include "protos/main.h"
#include "snd.h"

void WaitSema(s32);
void SignalSema(s32);

#define REG(a) (*(volatile u32 *)(a))
#define SREG(a) (*(volatile s32 *)(a))

s32 viBufRestartDMA(ViBuf *s) {
    u32 st;
    u32 madr;
    u32 qwc;
    u32 chcr;
    u32 tadr;
    u32 pcr;
    s32 n;
    s32 n2;
    s32 size;
    s32 id;
    s32 a;
    s32 b;
    u32 ie;

    st = s->env[7];
    pcr = st & 0x7F;
    madr = s->env[0] - (((st >> 16) & 3) + ((st >> 8) & 0xF)) * 16;
    qwc = s->env[2] + (((st >> 16) & 3) + ((st >> 8) & 0xF));
    chcr = s->env[3] | 0x100;
    tadr = s->env[1];
    WaitSema(s->sema);
    if (madr < (u32)s->data) {
        n = s->n;
        size = n << 11;
        qwc = ((u32)s->data - madr) >> 4;
        tadr = (u32)s->tag & 0x0FFFFFFF;
        madr += size;
        id = 0;
        if (s->env[0] != (u32)s->data) {
            id = (s->env[0] == (u32)s->data + size) ? 0 : 3;
        }
        chcr = (s->env[3] & 0x0FFFFFFF) | (id << 28) | 0x100;
        if ((n - s->dmaStart) % n < 0 || (n - s->dmaStart) % n >= s->dmaN) {
            s->dmaStart = n - 1;
            s->dmaN++;
        }
    } else {
        a = getFIFOindex(s, s->env[0]);
        b = getFIFOindex(s, madr);
        if (a != b) {
            n2 = s->n;
            qwc = ((u32)s->data + (a << 11) - madr) >> 4;
            tadr = ((a << 4) + (u32)s->tag) & 0x0FFFFFFF;
            id = ((u32)s->data + (s->env[0] - (u32)s->data) % (u32)(n2 << 11) == (u32)s->data + ((s->dmaStart + s->dmaN) % n2 << 11)) ? 0 : 3;
            chcr = (s->env[3] & 0x0FFFFFFF) | (id << 28) | 0x100;
            if ((b + n2 - s->dmaStart) % n2 < 0 || (b + n2 - s->dmaStart) % n2 >= s->dmaN) {
                s->dmaStart = b;
                s->dmaN++;
            }
        }
    }
    if (s->env[4] != 0 && s->env[5] != 0) {
        REG(0x1000B010) = s->env[4];
        REG(0x1000B020) = s->env[5];
        setD3_CHCR(s->env[6] | 0x100);
    }
    if (s->dmaN != 0) {
        do {
        } while (SREG(0x10002010) < 0);
        REG(0x10002000) = pcr;
        do {
        } while (SREG(0x10002010) < 0);
    }
    REG(0x1000B410) = madr;
    REG(0x1000B430) = tadr;
    REG(0x1000B420) = qwc;
    if (s->dmaN != 0) {
        ie = chcr & 0x100;
        if (ie == 0) {
            REG(0x1000F590) = 0x10000;
        }
        setD4_CHCR(chcr);
        if (ie == 0) {
            REG(0x1000F590) = 0;
            __asm__ volatile("sync");
        }
    }
    REG(0x10002010) = s->env[8];
    s->isActive = 1;
    SignalSema(s->sema);
    return 1;
}
