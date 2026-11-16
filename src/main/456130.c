#include "common.h"
#include "main/n08.h"

void sceDmaSendN(sceDmaChan *, s32, s32);

void GsToSprDmaSendN(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 base;

    base = 0x90000000;
    gsDmaChanToSpr->sadr = (void *)((arg0 + base) | 0x80000000);
    *(volatile u32 *)0x1000E000 = (*(volatile u32 *)0x1000E000 & ~2) | (arg3 << 1);
    sceDmaSendN(gsDmaChanToSpr, arg1, arg2);
}
