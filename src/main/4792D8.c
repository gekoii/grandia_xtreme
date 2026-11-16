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
    s32 bp;
    s32 ctrl;
} IpuState;


void sceIpuStopDMA(IpuState *st) {
    _sceIpuSetToChcr(1);
    st->to_madr = *(volatile s32 *)0x1000B410;
    st->to_tadr = *(volatile s32 *)0x1000B430;
    st->to_qwc = *(volatile s32 *)0x1000B420;
    st->to_chcr = *(volatile s32 *)0x1000B400;
    while (*(volatile s32 *)0x10002010 & 0xF0) {
    }
    _sceIpuSetFromChcr(0);
    st->from_madr = *(volatile s32 *)0x1000B010;
    st->from_qwc = *(volatile s32 *)0x1000B020;
    st->from_chcr = *(volatile s32 *)0x1000B000;
    st->bp = *(volatile s32 *)0x10002020;
    st->ctrl = *(volatile s32 *)0x10002010;
}
