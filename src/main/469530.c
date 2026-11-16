#include "common.h"
#include "protos/main.h"
#include "main/n11.h"


#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000)
#define PHYS(x) ((u32)(x) & 0x0FFFFFFF)
#define NBLK(s) (((s)->width + 63) / 64)

void MovIpartSetImageTag(MovDisp *scr, u32 base, u32 img, s32 flag) {
    MovRect a;
    MovRect b;
    MovRect c;
    MovRect d;
    u32 *q;
    s32 w16;
    s32 h16;
    s32 nw;
    s32 nh;
    s32 i;
    s32 j;
    s32 n;
    s32 m;

    q = (u32 *)UNCACHED(base);
    nw = scr->imageWidth >> 4;
    nh = scr->imageHeight >> 4;
    w16 = scr->imageWidth << 4;
    h16 = scr->imageHeight << 4;
    b.x = 0;
    b.y = 0;
    b.w = w16;
    b.h = h16;
    d.x = 0;
    d.y = 0;
    d.w = w16;
    d.h = h16;
    if (flag == 0) {
        a.x = 0;
        a.y = 0;
        a.w = w16;
        a.h = h16;
        c.x = 0;
        c.y = 0;
        c.w = w16;
        c.h = h16;
    } else {
        a.x = 0;
        a.y = 0x1C00;
        a.w = w16;
        a.h = h16;
        c.x = 0;
        c.y = 0x1C00;
        c.w = w16;
        c.h = h16;
    }

    n = NBLK(scr);
    m = n * 0x380;
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsBitBltBuf(q, m, n, 0);
    q = MovGsTrxReg(q, 0x10, 0x10);
    for (i = 0; i < nw; i++) {
        for (j = 0; j < nh; j++) {
            q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 4);
            q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
            q = MovGsTrxPos(q, 0, i * 16, j * 16);
            q = MovGsTrxDir(q, 0);
            q = MovGsGifTag(q, 0, 0, 2, 0, 0, 0, 0x40);
            q = MovGsDmaTag(q, 0, PHYS(img), 0, 3, 0, 0x40);
            img += 0x400;
        }
    }

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsBitBltBuf(q, NBLK(scr) * 0x380 + 0x1180, 1, 0);
    q = MovGsTrxReg(q, 0x40, 1);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 4);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsTrxPos(q, 0, 0, 0);
    q = MovGsTrxDir(q, 0);
    q = MovGsGifTag(q, 0, 0, 2, 0, 0, 0, 4);
    q = MovGsDmaTag(q, 0, PHYS(UNCACHED(movTrpClut)), 0, 3, 0, 4);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsTexFlush(q);
    q = MovGsTex0(q, 0, 1, 0x14, 0xA, 0xA, 0, 1, NBLK(scr) * 0x380 + 0x1180, 0, 1, 0, 1);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsBitBltBuf(q, NBLK(scr) * 0x380 + 0x1180, 0xA, 0x14);
    q = MovGsTrxReg(q, 0x280, 0x34);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 4);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsTrxPos(q, 0, 0, 0);
    q = MovGsTrxDir(q, 0);
    q = MovGsGifTag(q, 0, 0, 2, 0, 0, 0, 0x410);
    movPktStripSrc = ((q - (u32 *)UNCACHED(base))) | 0x08000000;
    q = MovGsDmaTag(q, 0, PHYS(UNCACHED(movTrpStrip)), 0, 3, 0, 0x410);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsBitBltBuf(q, NBLK(scr) * 0x380 + 0x1220, 6, 0x14);
    q = MovGsTrxReg(q, 0x180, 0x2DC);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 4);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsTrxPos(q, 0, 0, 0);
    q = MovGsTrxDir(q, 0);
    q = MovGsGifTag(q, 0, 0, 2, 0, 0, 0, 0x2250);
    q = MovGsDmaTag(q, 0, PHYS(UNCACHED(movTrpScroll)), 0, 3, 0, 0x2250);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsBitBltBuf(q, NBLK(scr) * 0x380 + 0x1460, 0xA, 0x14);
    q = MovGsTrxReg(q, 0x280, 0x1C1);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 4);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsTrxPos(q, 0, 0, 0);
    q = MovGsTrxDir(q, 0);
    q = MovGsGifTag(q, 0, 0, 2, 0, 0, 0, 0x2314);
    movPktIpartPictureSrc = ((q - (u32 *)UNCACHED(base))) | 0x08000000;
    q = MovGsDmaTag(q, 0, PHYS(UNCACHED(movTrpPicture)), 0, 3, 0, 0x2314);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsBitBltBuf(q, NBLK(scr) * 0x380 + 0x16E0, 4, 0x14);
    q = MovGsTrxReg(q, 0x100, 0x16);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 4);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 2);
    q = MovGsTrxPos(q, 0, 0, 0);
    q = MovGsTrxDir(q, 0);
    q = MovGsGifTag(q, 0, 0, 2, 0, 0, 0, 0xB0);
    q = MovGsDmaTag(q, 0, PHYS(UNCACHED(movTrpLabel)), 0, 3, 0, 0xB0);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 8);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 7);
    q = MovGsTex1(q, 0, 0, 0, 0, 0, 0, 0);
    q = MovGsTex0(q, NBLK(scr) * 0x380, NBLK(scr), 0, 0xA, 0xA, 0, 1, 0, 0, 0, 0, 0);
    q = MovGsSpriteTex(q, &a, &b);
    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 0xB);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 0xA);
    q = MovGsColClamp(q, 1);

    a.x = 0;
    if (flag == 0) {
        a.y = 0;
    } else {
        a.y = 0x1C00;
    }
    a.w = 0x2800;
    a.h = 0x1C00;
    q = MovGsSpriteBlend(q, &a, 0x80000000);
    movPktIpartDimAlpha = ((q - (u32 *)UNCACHED(base))) | 0x08000000;
    q = MovGsAlpha(q, 1, 2, 2, 0, 0x80);
    q = MovGsSpriteBlend(q, &a, 0x80000000);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 9);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 8);
    q = MovGsTest(q, 1, 6, 0, 0, 0, 0, 1, 1);
    q = MovGsTex1(q, 0, 0, 0, 0, 0, 0, 0);
    q = MovGsTex0(q, NBLK(scr) * 0x380 + 0x1220, 6, 0x14, 0xA, 0xA, 1, 1, NBLK(scr) * 0x380 + 0x1180, 0, 1, 0, 0);

    b.x = 0;
    b.y = 0;
    b.w = 0x1540;
    b.h = 0;
    a.x = 0x11E0;
    if (flag == 0) {
        a.y = 0x180;
    } else {
        a.y = 0x1D80;
    }
    a.w = 0x1540;
    a.h = 0;
    movPktIpartColumn = ((q - (u32 *)UNCACHED(base)) + 4) | 0x08000000;
    q = MovGsSpriteTex(q, &a, &b);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 0xE);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 0xD);
    q = MovGsTest(q, 1, 6, 0, 0, 0, 0, 1, 1);
    q = MovGsTex1(q, 0, 0, 0, 0, 0, 0, 0);
    q = MovGsTex0(q, NBLK(scr) * 0x380 + 0x1180, 0xA, 0x14, 0xA, 0xA, 1, 1, NBLK(scr) * 0x380 + 0x1180, 0, 1, 0, 0);

    b.x = 0;
    b.y = 0;
    b.w = 0x2800;
    b.h = 0x1A0;
    a.x = 0;
    if (flag == 0) {
        a.y = 0x1870;
    } else {
        a.y = 0x3470;
    }
    a.w = 0;
    a.h = 0x1A0;
    movPktIpartStripL = ((q - (u32 *)UNCACHED(base)) + 4) | 0x08000000;
    q = MovGsSpriteTex(q, &a, &b);

    d.x = 0;
    d.y = 0x1A0;
    d.w = 0x2800;
    d.h = 0x1A0;
    c.x = 0x2800;
    if (flag == 0) {
        c.y = 0x18F0;
    } else {
        c.y = 0x34F0;
    }
    c.w = 0;
    c.h = 0x1A0;
    movPktIpartStripR = ((q - (u32 *)UNCACHED(base)) + 4) | 0x08000000;
    q = MovGsSpriteTex(q, &c, &d);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 0xB);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 0xA);
    q = MovGsTest(q, 1, 6, 0, 0, 0, 0, 1, 1);
    q = MovGsTex1(q, 0, 0, 0, 0, 0, 0, 0);
    q = MovGsColClamp(q, 1);
    q = MovGsAlpha(q, 0, 2, 2, 1, 0x6C);
    q = MovGsTex0(q, NBLK(scr) * 0x380 + 0x1460, 0xA, 0x14, 0xA, 0xA, 1, 1, NBLK(scr) * 0x380 + 0x1180, 0, 1, 0, 0);

    b.x = 0;
    b.y = 0;
    b.w = 0x2580;
    b.h = 0;
    a.x = 0x200;
    if (flag == 0) {
        a.y = 0;
    } else {
        a.y = 0x1C00;
    }
    a.w = 0x2580;
    a.h = 0;
    movPktIpartPicture = ((q - (u32 *)UNCACHED(base)) + 4) | 0x08000000;
    q = MovGsSpriteTexBlend(q, &a, &b);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 0xB);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 0xA);
    q = MovGsTest(q, 1, 6, 0, 0, 0, 0, 1, 1);
    q = MovGsTex1(q, 0, 0, 0, 0, 0, 0, 0);
    q = MovGsColClamp(q, 1);
    movPktIpartLabelAlpha = ((q - (u32 *)UNCACHED(base))) | 0x08000000;
    q = MovGsAlpha(q, 0, 2, 2, 1, 0x20);
    q = MovGsTex0(q, NBLK(scr) * 0x380 + 0x16E0, 4, 0x14, 0xA, 0xA, 1, 1, NBLK(scr) * 0x380 + 0x1180, 0, 1, 0, 0);

    b.x = 0;
    b.y = 0;
    b.w = 0x1000;
    b.h = 0x160;
    a.x = 0xC00;
    if (flag == 0) {
        a.y = 0xD50;
    } else {
        a.y = 0x2950;
    }
    a.w = 0x1000;
    a.h = 0x160;
    q = MovGsSpriteTexBlend(q, &a, &b);

    q = MovGsDmaTag(q, 0, 0, 0, 1, 0, 6);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 0, 5);
    movPktFadeAlpha = ((q - (u32 *)UNCACHED(base))) | 0x08000000;
    q = MovGsAlpha(q, 1, 2, 2, 0, 0x80);

    a.x = 0;
    if (flag == 0) {
        a.y = 0;
    } else {
        a.y = 0x1C00;
    }
    a.w = 0x2800;
    a.h = 0x1C00;
    q = MovGsSpriteBlend(q, &a, 0x80000000);
    q = MovGsDmaTag(q, 0, 0, 0, 7, 0, 3);
    q = MovGsGifTag(q, 0xE, 1, 0, 0, 0, 1, 2);
    q = MovGsTexFlush(q);
    q = MovGsTest(q, 0, 0, 0, 0, 0, 0, 1, 1);
}
