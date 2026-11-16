/* Port-only C for AmapTilesFixWallGimmicks (0x00607E70), which the decompilation still has in assembly. */
#include "common.h"
#include "field/n21.h"

#define TILE(w, x, y) (w)->tiles[(y) * (w)->width + (x)]

/* For each wall gimmick (types 3, 4 and 0x18), re-tiles the two wall tiles
   beside it by its facing (bits 6-7). */
void AmapTilesFixWallGimmicks(AmapWork *w) {
    AmapPlace *g;
    AmapPlace *e;
    s32 i;
    s32 t;
    s32 x;
    s32 y;
    u8 a;
    u8 b;

    g = w->gimmicks;
    for (i = 0; i < w->gimmickCount; i++) {
        e = &g[i];
        t = e->type & 0x1F;
        if (t < 3) {
            continue;
        }
        if (t >= 5 && t != 0x18) {
            continue;
        }
        x = e->x;
        y = e->y;
        switch (e->type >> 6) {
        case 0:
            a = TILE(w, x - 1, y - 1);
            b = TILE(w, x, y - 1);
            if (a == 0xC3) a = 0xD6;
            if (a == 0x01) a = 0x14;
            if (a == 0x04) a = 0x14;
            if (a == 0x17) a = 0x14;
            if (a == 0x18) a = 0x14;
            if (b == 0x83) b = 0x95;
            if (b == 0x81) b = 0x93;
            if (b == 0x84) b = 0x93;
            if (b == 0x97) b = 0x93;
            if (b == 0x98) b = 0x93;
            TILE(w, x - 1, y - 1) = a;
            TILE(w, x, y - 1) = b;
            break;
        case 1:
            a = TILE(w, x, y - 1);
            b = TILE(w, x, y);
            if (a == 0x03) a = 0x16;
            if (a == 0x41) a = 0x54;
            if (a == 0x44) a = 0x54;
            if (a == 0x57) a = 0x54;
            if (a == 0x58) a = 0x54;
            if (b == 0xC3) b = 0xD5;
            if (b == 0xC1) b = 0xD3;
            if (b == 0xC4) b = 0xD3;
            if (b == 0xD7) b = 0xD3;
            if (b == 0xD8) b = 0xD3;
            TILE(w, x, y - 1) = a;
            TILE(w, x, y) = b;
            break;
        case 2:
            a = TILE(w, x - 1, y);
            b = TILE(w, x, y);
            if (a == 0x03) a = 0x15;
            if (a == 0x01) a = 0x13;
            if (a == 0x04) a = 0x13;
            if (a == 0x17) a = 0x13;
            if (a == 0x18) a = 0x13;
            if (b == 0x43) b = 0x56;
            if (b == 0x81) b = 0x94;
            if (b == 0x84) b = 0x94;
            if (b == 0xD7) b = 0xD4;
            if (b == 0xD8) b = 0xD4;
            TILE(w, x - 1, y) = a;
            TILE(w, x, y) = b;
            break;
        case 3:
            a = TILE(w, x - 1, y - 1);
            b = TILE(w, x - 1, y);
            if (a == 0x43) a = 0x55;
            if (a == 0x41) a = 0x53;
            if (a == 0x44) a = 0x53;
            if (a == 0x57) a = 0x53;
            if (a == 0x58) a = 0x53;
            if (b == 0x83) b = 0x96;
            if (b == 0xC1) b = 0xD4;
            if (b == 0xC4) b = 0xD4;
            if (b == 0xD7) b = 0xD4;
            if (b == 0xD8) b = 0xD4;
            TILE(w, x - 1, y - 1) = a;
            TILE(w, x - 1, y) = b;
            break;
        }
    }
}
