#include "common.h"
#include "protos/main.h"
#include "main/n08.h"


void GsMfifoStop(void) {
    sceDmaChan *a;
    sceDmaChan *b;
    tD_CHCR x;
    *(volatile s32 *)0x1000F590 = 0x10000;
    a = gsDmaChanFromSpr;
    b = gsDmaChanVif1;
    x = a->chcr;
    x.STR = 0;
    a->chcr = x;
    x = b->chcr;
    x.STR = 0;
    b->chcr = x;
    gsDmaEnv.mfd = 0;
    gsDmaEnv.rbadr = NULL;
    gsDmaEnv.rbmsk = 0;
    sceDmaPutEnv(&gsDmaEnv);
    *(volatile s32 *)0x1000F590 = 0;
    __asm__ volatile("sync");
}
