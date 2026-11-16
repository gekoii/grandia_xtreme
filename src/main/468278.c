#include "common.h"
#include "protos/main.h"
#include "main/n11.h"


void MovSetImageTag(MovDisp *tex, s32 buf, s32 addr, s32 flag) {
    MovRect r0;
    MovRect r1;
    MovRect r2;
    MovRect r3;
    s32 w;
    s32 h;
    s32 nx;
    s32 ny;
    s32 p;
    s32 size;
    s32 n;
    s32 n2;
    s32 i;
    s32 j;

    w = tex->imageWidth * 16;
    nx = tex->imageWidth >> 4;
    p = (buf & 0x0FFFFFFF) | 0x20000000;
    ny = tex->imageHeight >> 4;
    h = tex->imageHeight * 16;
    r1.x = 0;
    r1.y = 0;
    r1.w = w;
    r1.h = h;
    r3.x = 0;
    r3.y = 0;
    r3.w = w;
    r3.h = h;
    if (flag == 0) {
        r0.x = 0;
        r0.y = 0;
        r0.w = w;
        r0.h = h;
        r2.x = 0;
        r2.y = 0;
        r2.w = w;
        r2.h = h;
    } else {
        r0.x = 0;
        r0.y = 0x1C00;
        r0.w = w;
        r0.h = h;
        r2.x = 0;
        r2.y = 0x1C00;
        r2.w = w;
        r2.h = h;
    }
    n = (tex->width + 63) / 64;
    size = n * 0x380;
    p = MovGsDmaTag(p, 0, 0, 0, 1, 0, 3);
    p = MovGsGifTag(p, 0xE, 1, 0, 0, 0, 0, 2);
    p = MovGsBitBltBuf(p, size, n, 0);
    p = MovGsTrxReg(p, 0x10, 0x10);
    for (i = 0; i < nx; i++) {
        for (j = 0; j < ny; j++) {
            p = MovGsDmaTag(p, 0, 0, 0, 1, 0, 4);
            p = MovGsGifTag(p, 0xE, 1, 0, 0, 0, 0, 2);
            p = MovGsTrxPos(p, 0, i * 16, j * 16);
            p = MovGsTrxDir(p, 0);
            p = MovGsGifTag(p, 0, 0, 2, 0, 0, 0, 0x40);
            p = MovGsDmaTag(p, 0, addr & 0x0FFFFFFF, 0, 3, 0, 0x40);
            addr += 0x400;
        }
    }
    p = MovGsDmaTag(p, 0, 0, 0, 1, 0, 0x10);
    p = MovGsGifTag(p, 0xE, 1, 0, 0, 0, 0, 0xF);
    p = MovGsTexFlush(p);
    p = MovGsTex1(p, 0, 0, 0, 0, 0, 0, 0);
    n2 = (tex->width + 63) / 64;
    p = MovGsTex0(p, n2 * 0x380, n2, 0, 10, 10, 0, 1, 0, 0, 0, 0, 0);
    p = MovGsSpriteTex(p, &r0, &r1);
    p = MovGsTex1(p, 0, 0, 0, 0, 0, 0, 0);
    p = MovGsAlpha(p, 0, 2, 2, 1, 0x20);
    p = MovGsSpriteTex(p, &r2, &r3);
    p = MovGsDmaTag(p, 0, 0, 0, 7, 0, 7);
    p = MovGsGifTag(p, 0xE, 1, 0, 0, 0, 1, 6);
    p = MovGsColClamp(p, 1);
    movPktFadeAlpha = ((p - ((buf & 0x0FFFFFFF) | 0x20000000)) >> 2) | 0x08000000;
    p = MovGsAlpha(p, 1, 2, 2, 0, 0x80);
    r0.x = 0;
    if (flag == 0) {
        r0.y = 0;
    } else {
        r0.y = 0x1C00;
    }
    r0.w = 0x2800;
    r0.h = 0x1C00;
    MovGsSpriteBlend(p, &r0, 0x80000000);
}
