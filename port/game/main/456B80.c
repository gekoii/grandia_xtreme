/* Port-only C for GsTexPacketSet (0x00456B80), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"

void GsTexPacketSet(GsTexPacket *pkt, GphTex *tex, u32 tbp, u32 cbp, u32 csa, u32 wms, u32 wmt, u32 minu,
                    u32 maxu, u32 minv, u32 maxv, u32 tfx);

/* A mip level's size in GS blocks (64 at least) */
static u32 MipBlocks(u32 w, u32 h, u32 bpp) {
    u32 sz = (w * h * bpp) >> 5;

    return sz > 0x3F ? sz : 0x40;
}

/* A mip level's buffer width (64 at least) */
static u32 MipWidth(u32 w) {
    return w > 0x3F ? w : 0x40;
}

/* Fills a texture packet (0x140 bytes): the GsPacketHead (cnt tag, id
   0xEEEE0006, VIF DIRECT, an A+D GIF tag) and TEX0_1, TEX1_1, CLAMP_1,
   MIPTBP1_1, MIPTBP2_1 and TEXFLUSH, then what the upload needs: the mip
   count, levels' data, base pointers and widths, PSM, size, tex +0x38,
   the CLUTs' data and base pointers and the CLUT size. tex fields by
   offset: +0x14 is the mip count, +0x3C the CLD flag, +0x68 MMIN's flag,
   +0x70 L, +0x74 K, +0x1C/+0x40 the mip/CLUT data. The 12 arguments are
   EABI's 8 registers and 4 stack words. */
void GsTexPacketSet(GsTexPacket *pkt, GphTex *tex, u32 tbp, u32 cbp, u32 csa, u32 wms, u32 wmt, u32 minu,
                    u32 maxu, u32 minv, u32 maxv, u32 tfx) {
    u32 *t = (u32 *)tex;
    u8 *p = pkt->data;
    u32 *pw = (u32 *)p;
    u64 *ad = (u64 *)(p + 0x30);
    u32 psm = t[0x10 / 4];
    u32 w, h, nmip, bpp;
    u32 clutSize, clutW, clutH;
    u32 lcm, mmin;
    u32 tbps[7];
    u32 tbws[7];
    s32 i;

    pw[0x1C / 4] = 0x50000007;
    pw[0x10 / 4] = 0;
    pw[0x14 / 4] = 0;
    pw[0x18 / 4] = 0;
    *(u64 *)(p + 0x00) = 0x10000008;
    *(u64 *)(p + 0x08) = 0xEEEE0006;
    *(u64 *)(p + 0x20) = 0x1000000000008006ULL;
    *(u64 *)(p + 0x28) = 0xE;
    if (psm == 0x13) {
        clutSize = 0x100;
        clutW = 0x10;
        clutH = 0x10;
    } else {
        clutSize = 0x40;
        clutW = 8;
        clutH = 2;
    }
    w = t[0x00 / 4];
    nmip = t[0x14 / 4];

    /* TEX0_1 */
    ad[0] = (u64)(tbp >> 6) + ((u64)(cbp + clutSize * csa) >> 6 << 37) + ((u64)t[0x08 / 4] << 26) +
            (1ULL << 34) + ((u64)(w >> 6) << 14) + ((u64)psm << 20) + ((u64)t[0x0C / 4] << 30) +
            ((u64)tfx << 35) + ((u64)(t[0x3C / 4] != 0) << 61);
    ad[1] = 6;

    /* TEX1_1 */
    if (nmip != 0) {
        lcm = 0;
        mmin = t[0x68 / 4] != 0 ? 5 : 4;
    } else {
        lcm = 1;
        mmin = 1;
    }
    ad[2] = (lcm | ((u64)t[0x74 / 4] << 32)) + ((u64)nmip << 2) + ((u64)mmin << 6) + ((u64)t[0x70 / 4] << 19) +
            0x20;
    ad[3] = 0x14;

    /* CLAMP_1 */
    ad[4] = (u64)wms + ((u64)maxv << 34) + ((u64)maxu << 14) + ((u64)minu << 4) + ((u64)wmt << 2) +
            ((u64)minv << 24);
    ad[5] = 8;

    h = t[0x04 / 4];
    if (psm == 2) {
        bpp = 4;
    } else if (psm < 2) {
        bpp = 0x20;
    } else if (psm == 0x13) {
        bpp = 8;
    } else {
        bpp = 4;
    }
    tbps[0] = tbp;
    tbws[0] = w;
    for (i = 1; i < 7; i++) {
        tbps[i] = tbps[i - 1] + MipBlocks(w >> (i - 1), h >> (i - 1), bpp);
        tbws[i] = MipWidth(w >> i);
    }

    /* MIPTBP1_1, MIPTBP2_1 */
    ad[6] = (u64)(tbps[1] >> 6) + ((u64)(tbws[1] >> 6) << 14) + ((u64)(tbps[2] >> 6) << 20) +
            ((u64)(tbws[2] >> 6) << 34) + ((u64)(tbps[3] >> 6) << 40) + ((u64)(tbws[3] >> 6) << 54);
    ad[7] = 0x34;
    ad[8] = (u64)(tbps[4] >> 6) + ((u64)(tbws[4] >> 6) << 14) + ((u64)(tbps[5] >> 6) << 20) +
            ((u64)(tbws[5] >> 6) << 34) + ((u64)(tbps[6] >> 6) << 40) + ((u64)(tbws[6] >> 6) << 54);
    ad[9] = 0x36;
    /* TEXFLUSH */
    ad[10] = 0;
    ad[11] = 0x3F;

    pw[0x90 / 4] = nmip;
    for (i = 0; i < 7; i++) {
        pw[0x94 / 4 + i] = t[0x1C / 4 + i];
        pw[0xB0 / 4 + i] = tbps[i];
        pw[0xCC / 4 + i] = tbws[i];
    }
    pw[0xE8 / 4] = t[0x10 / 4];
    pw[0xEC / 4] = w;
    pw[0xF0 / 4] = h;
    pw[0xF4 / 4] = t[0x38 / 4];
    for (i = 0; i < 8; i++) {
        pw[0xF8 / 4 + i] = t[0x40 / 4 + i];
        pw[0x118 / 4 + i] = cbp + clutSize * i;
    }
    pw[0x138 / 4] = clutW;
    pw[0x13C / 4] = clutH;
    grx_sync();
}
