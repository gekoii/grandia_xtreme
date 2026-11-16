#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 viBufReset(ViBuf *arg0) {
    s32 i;

    i = 0;
    arg0->isActive = 1;
    arg0->dmaStart = 0;
    arg0->dmaN = 0;
    arg0->readBytes = 0;
    arg0->totalBytes = 0;
    arg0->env[0] = 0;
    arg0->env[1] = 0;
    arg0->env[2] = 0;
    arg0->env[3] = 0;
    for (; i < arg0->n; i++) {
        scTag2(&arg0->tag[i], (u32)(arg0->data + (i << 11)), 3, 0x80);
    }
    scTag2(&arg0->tag[i], (u32)arg0->tag & 0x0FFFFFFF, 2, 0);
    *(volatile s32 *)0x1000B420 = 0;
    *(volatile u32 *)0x1000B410 = (u32)arg0->data & 0x0FFFFFFF;
    *(volatile u32 *)0x1000B430 = (u32)arg0->tag & 0x0FFFFFFF;
    *(volatile s32 *)0x1000F590 = 0x10000;
    setD4_CHCR(5);
    *(volatile s32 *)0x1000F590 = 0;
    __asm__ volatile("sync");
    return 1;
}
