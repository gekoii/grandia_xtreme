#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

void sceDmaSendI(sceDmaChan *, s32, s32);

void GsToSprDmaSendI(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4) {
    gsDmaChanToSpr->sadr = (void *)((arg0 + 0x90000000) | 0x80000000);
    gsDmaEnv.tqwc = arg2;
    gsDmaEnv.sqwc = arg3;
    sceDmaPutEnv(&gsDmaEnv);
    *(volatile u32 *)0x1000E000 &= 0xFFFFFFFD;
    sceDmaSendI(gsDmaChanToSpr, arg1, arg4);
}
