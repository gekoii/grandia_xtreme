/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct {
    u64 NLOOP : 15;
    u64 EOP : 1;
    u64 pad16 : 16;
    u64 id : 14;
    u64 PRE : 1;
    u64 PRIM : 11;
    u64 FLG : 2;
    u64 NREG : 4;
    u64 REGS0 : 4;
    u64 REGS1 : 4;
    u64 REGS2 : 4;
    u64 REGS3 : 4;
    u64 REGS4 : 4;
    u64 REGS5 : 4;
    u64 REGS6 : 4;
    u64 REGS7 : 4;
    u64 REGS8 : 4;
    u64 REGS9 : 4;
    u64 REGS10 : 4;
    u64 REGS11 : 4;
    u64 REGS12 : 4;
    u64 REGS13 : 4;
    u64 REGS14 : 4;
    u64 REGS15 : 4;
} GifTag;

typedef struct {
    u32 vifcode[4];
    GifTag giftag;
    u64 bitbltbuf;
    u64 bitbltbufaddr;
    u64 trxpos;
    u64 trxposaddr;
    u64 trxreg;
    u64 trxregaddr;
    u64 finish;
    u64 finishaddr;
    u64 trxdir;
    u64 trxdiraddr;
} StoreImage;

s32 sceGsSetDefStoreImage(StoreImage *sp, s16 sbp, s16 sbw, s16 spsm, s16 x, s16 y, s16 w, s16 h) {
    sp->vifcode[0] = 0;
    sp->vifcode[1] = 0x06008000;
    sp->vifcode[2] = 0x13000000;
    sp->vifcode[3] = 0x50000006;
    *(u128 *)&sp->giftag = 0;
    sp->giftag.NLOOP = 5;
    sp->giftag.EOP = 1;
    sp->giftag.NREG = 1;
    sp->giftag.REGS0 = 0xE;
    sp->bitbltbuf = (u64)sbp | ((u64)sbw << 16) | ((u64)spsm << 24);
    sp->bitbltbufaddr = 0x50;
    sp->trxpos = (u64)x | ((u64)y << 16);
    sp->trxposaddr = 0x51;
    sp->trxreg = (u64)w | ((u64)h << 32);
    sp->trxregaddr = 0x52;
    sp->finish = 0;
    sp->finishaddr = 0x61;
    sp->trxdir = 1;
    sp->trxdiraddr = 0x53;
    __asm__ volatile("sync");
    return sizeof(StoreImage) >> 4;
}
