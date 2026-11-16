#include "common.h"

typedef struct {
    u32 f0;
    u32 f4;
    u32 f8;
    u32 fC;
    u64 f10;
    u64 f18;
    u64 f20;
    u64 f28;
    u64 f30;
    u64 f38;
    u64 f40;
    u64 f48;
    u64 f50;
    u64 f58;
    u64 f60;
    u64 f68;
} Pkt;

s32 GsImageXferPacketSet(Pkt *p, u32 dbp, u32 dbw, u32 psm, u32 x, u32 y, u32 w, u32 h) {
    s32 qwc;

    qwc = 0xFFFF;
    switch (psm) {
    case 0:
        qwc = (s32)(w * h) >> 2;
        break;
    case 1:
        qwc = (s32)(w * h * 3) >> 4;
        break;
    case 2:
        qwc = (s32)(w * h) >> 3;
        break;
    case 19:
    case 27:
        qwc = (s32)(w * h) >> 4;
        break;
    case 20:
    case 36:
    case 44:
        qwc = (s32)(w * h) >> 5;
        break;
    }
    p->fC = ((qwc + 6) & 0xFFFF) | 0x50000000;
    p->f10 = (0x10000000ULL << 32) | 4;
    p->f18 = 0xE;
    p->f20 = ((u64)dbp << 32) | ((u64)dbw << 48) | ((u64)psm << 56);
    p->f28 = 0x50;
    p->f30 = (u64)x | ((u64)y << 16);
    p->f38 = 0x51;
    p->f40 = ((u64)h << 32) | (u64)w;
    p->f48 = 0x52;
    p->f58 = 0x53;
    p->f60 = (u64)(u32)qwc | ((0x08000000ULL << 32) | 0x8000);
    p->f0 = 0;
    p->f4 = 0;
    p->f8 = 0;
    p->f50 = 0;
    p->f68 = 0;
    __asm__ volatile("sync");
    return qwc;
}
