#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

typedef struct {
    u64 tag;
    u64 unk8;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u64 tex1;
    u64 test;
    u64 prim;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
} TextPkt;



#define PKT ((TextPkt *)0x70003F60)

void DbgTextDrawChars(void) {
    s32 x0;
    s32 y0;
    s32 x;
    u64 color;
    u32 i;
    s32 y;
    u32 c;
    s32 ch;
    s32 u;
    s32 v;

    if (dbgTextBuf.count == 0) {
        return;
    }
    x0 = (dbgTextOfsX + 0x800) << 4;
    y0 = (dbgTextOfsY + 0x800) << 4;
    x = x0;
    y = y0;
    color = dbgTextColors[0];
    *(u64 *)0x70003F88 = 0x535310;
    *(u64 *)0x70003F60 = 0x10000005;
    *(u64 *)0x70003F68 = 0;
    *(u32 *)0x70003F70 = 0;
    *(u32 *)0x70003F74 = 0;
    *(u32 *)0x70003F78 = 0;
    *(u32 *)0x70003F7C = 0x50000004;
    *(u64 *)0x70003F80 = 0x6400000000008001ULL;
    *(u64 *)0x70003F90 = 6;
    for (i = 0; i < dbgTextBuf.count; i++) {
        c = dbgTextBuf.buf[i];
        if (c > 0xFFFF) {
            if (c & 0x80000000) {
                color = dbgTextColors[c & 0xF];
            }
            if (c & 0x40000000) {
                dbgCharW = (c & 0xFFFF) << 4;
            }
            if (c & 0x20000000) {
                dbgCharH = (c & 0xFFFF) << 4;
            }
            if (c & 0x10000000) {
                x = x0 + ((c & 0xFFFF) << 4);
            }
            if (dbgTextBuf.buf[i] & 0x08000000) {
                y = y0 + ((dbgTextBuf.buf[i] & 0xFFFF) << 4);
            }
        } else {
            ch = (u8)dbgTextBuf.buf[i] - 0x20;
            v = ch & 0xFFF0;
            u = (ch & 0xF) << 4;
            *(u64 *)0x70003FA0 = ((u64)((v << 4) & 0xFF00) << 16) | ((u64)u << 4);
            *(u64 *)0x70003FA8 = x | (y << 16);
            *(u64 *)0x70003FB0 = ((u64)(((v + 0x10) << 4) & 0xFF00) << 16) | ((u64)(u + 0x10) << 4);
            *(u64 *)0x70003FB8 = (u32)((x + dbgCharW) | ((y + dbgCharH) << 16));
            *(u64 *)0x70003F98 = color;
            GsGifPacketSend((void *)0x70003F60);
            x += dbgCharW;
            GsFromSprDmaWait();
        }
    }
}
