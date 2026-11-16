#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[0x90];
    s32 mipCount;
    u32 mipAddr[7];
    u32 mipTbp[7];
    u32 mipTbw[7];
    u32 psm;
    u32 widths;
    u32 heights;
    u32 clutCount;
    u32 clutAddr[8];
    u32 clutTbp[8];
    u32 clutW;
    u32 clutH;
} Tex;

void GsMfifoSend(void *, s32);
s32 GsImageXferPacketSet(void *, u32, u32, u32, u32, u32, u32, u32);

void GsTexUpload(Tex *t) {
    u32 i;
    u32 addr;
    s32 q;
    u64 tq;

    i = 0;
    GsFromSprDmaWait();
    *(u64 *)0x70000800 = 0x10000003;
    *(u64 *)0x70000808 = 0xDDDD000F;
    *(s32 *)0x7000081C = 0x50000002;
    *(u64 *)0x70000820 = 0x1000000000000001;
    *(u64 *)0x70000828 = 0xE;
    *(u64 *)0x70000838 = 0x3F;
    *(s32 *)0x70000810 = 0;
    *(s32 *)0x70000814 = 0;
    *(s32 *)0x70000818 = 0;
    *(u64 *)0x70000830 = 0;
    GsMfifoSend((void *)0x70000800, 4);
    for (; i < t->clutCount; i++) {
        GsFromSprDmaWait();
        *(u64 *)0x70000800 = 0x10000007;
        *(u64 *)0x70000808 = 0xDDDD0000;
        q = GsImageXferPacketSet((void *)0x70000810, t->clutTbp[i] >> 6, 1, 0, 0, 0, t->clutW, t->clutH);
        addr = t->clutAddr[i];
        if (addr > 0x6FFFFFFF) {
            addr = (addr + 0x90000000) | 0x80000000;
        }
        tq = (u32)q;
        tq |= 0x30000000;
        *(u64 *)0x70000880 = ((u64)(((addr & 0x40000000) << 1) | ((addr & 0x1FFFFFFF) ^ ((addr & 0x40000000) >> 2))) << 32) | tq;
        *(u64 *)0x70000888 = 0xDDDD0001;
        GsMfifoSend((void *)0x70000800, 9);
    }
    for (i = 0; i < t->mipCount + 1; i++) {
        GsFromSprDmaWait();
        *(u64 *)0x70000800 = 0x10000007;
        *(u64 *)0x70000808 = 0xDDDD0002;
        q = GsImageXferPacketSet((void *)0x70000810, t->mipTbp[i] >> 6, t->mipTbw[i] >> 6, t->psm, 0, 0, t->widths >> i, t->heights >> i);
        addr = t->mipAddr[i];
        if (addr > 0x6FFFFFFF) {
            addr = (addr + 0x90000000) | 0x80000000;
        }
        tq = (u32)q;
        tq |= 0x30000000;
        *(u64 *)0x70000880 = ((u64)(((addr & 0x40000000) << 1) | ((addr & 0x1FFFFFFF) ^ ((addr & 0x40000000) >> 2))) << 32) | tq;
        *(u64 *)0x70000888 = 0xDDDD0003;
        GsMfifoSend((void *)0x70000800, 9);
    }
}
