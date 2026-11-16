#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 CreateSema(SemaParam *);

s32 viBufCreate(ViBuf *p, u32 base, u32 tags, s32 n, MovTimeStamp *items, s32 nitems) {
    SemaParam sema;
    s32 i;
    MovTimeStamp *it;
    u32 b;
    u32 t;
    u32 mask;
    volatile u32 *enw;

    p->tag = (void *)((tags & 0x0FFFFFFF) | 0x20000000);
    p->data = (void *)base;
    p->n = n;
    p->buffSize = n << 11;
    p->isActive = 1;
    i = 0;
    p->dmaStart = 0;
    p->dmaN = 0;
    p->readBytes = 0;
    p->totalBytes = 0;
    /* three nested do-while(0) wraps (here and around the init loop): phony
     * loops that multiply the refs and so set the allocation priorities */
    do {
        do {
            do {
                p->ts = items;
            } while (0);
        } while (0);
    } while (0);
    p->nTs = nitems;
    p->countTs = 0;
    p->wtTs = 0;
    do {
        do {
            do {
                if (nitems > 0) {
                /* goto loop: no loop-pass strength reduction of items[i] */
                loop:
                    it = &items[i];
                    i++;
                    it->pts = -1;
                    it->pos = 0;
                    it->dts = -1;
                    it->len = 0;
                    if (i < nitems) {
                        goto loop;
                    }
                }
            } while (0);
        } while (0);
    } while (0);
    sema.initCount = 1;
    sema.maxCount = 1;
    p->sema = CreateSema(&sema);
    for (i = 0; i < p->n; i++) {
        scTag2(tags + i * 0x10, base + i * 0x800, 3, 0x80);
    }
    mask = 0x0FFFFFFF;
    enw = (volatile u32 *)0x1000F590;
    scTag2(tags + i * 0x10, tags & mask, 2, 0);
    b = (u32)p->data;
    t = (u32)p->tag;
    *(volatile u32 *)0x1000B420 = 0;
    *(volatile u32 *)0x1000B410 = b & mask;
    *(volatile u32 *)0x1000B430 = t & mask;
    *enw = 0x10000;
    setD4_CHCR(5);
    *enw = 0;
    __asm__ volatile("sync");
    return 1;
}
