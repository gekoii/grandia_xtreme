#include "common.h"
#include "protos/main.h"
#include "main/n08.h"


void GsMfifoStart(s32 addr, s32 size) {
    sceDmaChan *a;
    sceDmaChan *b;
    tD_CHCR x;
    *(volatile s32 *)0x1000F590 = 0x10000;
    gsDmaEnv.mfd = 1;
    size = size - 1;
    size = size & 0xFFFFFFF0;
    gsDmaEnv.rbmsk = size;
    gsDmaEnv.rbadr = (void *)addr;
    sceDmaPutEnv(&gsDmaEnv);
    *(volatile s32 *)0x1000E000 = ((((*(volatile s32 *)0x1000E000 | 2) ^ 2) | 0x700) ^ 0x700) | 2;
    a = gsDmaChanFromSpr;
    x = a->chcr;
    a->madr = (void *)addr;
    x.DIR = 0;
    x.MOD = 0;
    x.STR = 0;
    a->chcr = x;
    b = gsDmaChanVif1;
    x = b->chcr;
    b->tadr = (void *)addr;
    x.DIR = 1;
    x.MOD = 1;
    x.STR = 1;
    b->chcr = x;
    *(volatile s32 *)0x1000F590 = 0;
    __asm__ volatile("sync");
}
