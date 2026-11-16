/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    s32 to_madr;
    s32 to_tadr;
    s32 to_qwc;
    s32 to_chcr;
    s32 from_madr;
    s32 from_qwc;
    s32 from_chcr;
    u32 bp;
    s32 ctrl;
} IpuState;


void sceIpuRestartDMA(IpuState *st) {
    s32 bp, n, qwc, madr;

    bp = st->bp & 0x7F;
    n = ((st->bp >> 16) & 3) + ((st->bp >> 8) & 0xF);
    madr = st->to_madr - n * 16;
    qwc = st->to_qwc + n;
    if (st->from_madr != 0 && st->from_qwc != 0) {
        *(volatile s32 *)0x1000B010 = st->from_madr;
        *(volatile s32 *)0x1000B020 = st->from_qwc;
        _sceIpuSetFromChcr(st->from_chcr | 0x100);
    }
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    *(volatile s32 *)0x10002000 = bp;
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    if (madr != 0 && qwc != 0) {
        *(volatile s32 *)0x1000B410 = madr;
        *(volatile s32 *)0x1000B430 = st->to_tadr;
        *(volatile s32 *)0x1000B420 = qwc;
        _sceIpuSetToChcr(st->to_chcr | 0x100);
    }
}
