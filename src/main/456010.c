#include "common.h"
#include "main/n08.h"

void sceDmaSendN(sceDmaChan *, s32, s32);

void GsFromSprDmaSendN(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 v = (arg1 - 0x70000000) | 0x80000000;
    volatile u32 *ctrl = (volatile u32 *)0x1000E000;
    sceDmaChan *q = gsDmaChanFromSpr;

    *ctrl = (*ctrl & 0xFFFFFF8D) | (arg3 << 1);
    q->sadr = (void *)v;
    sceDmaSendN(q, arg0, arg2);
}
