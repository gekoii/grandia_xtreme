#include "common.h"
#include "field/menu.h"

typedef struct {
    u32 top;
    u32 bottom;
} Grad;

extern const Grad fldMenuBarGradients[];

FldMenuQuad *FldMenuBarBuild(FldMenuQuad *p, s32 x, s32 y, s32 w, s32 h, s32 col) {
    s32 semi;
    u8 r, g, b;
    u32 c0, c1;

    if (col >= 0x80) {
        semi = 1;
        col -= 0x80;
    } else {
        semi = 0;
    }
    switch (col) {
    case 0:
        r = 0x80;
        g = 0x80;
        b = 0xFF;
        break;
    case 1:
        r = 0x83;
        g = 0xFD;
        b = 0x6F;
        break;
    case 2:
        r = 0xFF;
        g = 0x80;
        b = 0x80;
        break;
    case 11:
        r = 0xA0;
        g = 0xF0;
        b = 0xF0;
        break;
    case 12:
        r = 0xF0;
        g = 0x60;
        b = 0x40;
        break;
    case 13:
        r = 0xF0;
        g = 0xF0;
        b = 0xF0;
        break;
    case 14:
        r = 0xF0;
        g = 0xF0;
        b = 0x40;
        break;
    default:
        r = 0xFF;
        g = 0xFF;
        b = 0xFF;
        break;
    }
    if (col >= 0x14) {
        if (col >= 0x1C) {
            goto end; /* one shared return: an early return p duplicates the epilogue */
        }
        c0 = fldMenuBarGradients[col - 0x14].top;
        c1 = fldMenuBarGradients[col - 0x14].bottom;
        p->type = 5;
        p->v[0].r = c0 >> 24;
        p->v[0].g = c0 >> 16;
        p->v[0].b = c0 >> 8;
        p->v[0].a = 0x80;
        p->v[1].r = c1 >> 24;
        p->v[1].g = c1 >> 16;
        p->v[1].b = c1 >> 8;
        p->v[1].a = 0x80;
        p->v[2].r = c0 >> 24;
        p->v[2].g = c0 >> 16;
        p->v[2].b = c0 >> 8;
        p->v[2].a = 0x80;
        p->v[3].r = c1 >> 24;
        p->v[3].g = c1 >> 16;
        p->v[3].b = c1 >> 8;
        p->v[3].a = 0x80;
        p->v[0].x = x;
        p->v[0].y = y;
        p->v[1].x = x;
        p->v[1].y = y + 8;
        p->v[2].x = x + w;
        p->v[2].y = y;
        p->v[3].x = x + w;
        p->v[3].y = y + 8;
    } else {
        p->type = 5;
        if (semi) {
            p->type = 6;
        }
        p->v[0].r = r;
        p->v[0].g = g;
        p->v[0].b = b;
        p->v[0].a = 0x80;
        p->v[1].r = r >> 1;
        p->v[1].g = g >> 1;
        p->v[1].b = b >> 1;
        p->v[1].a = 0x80;
        p->v[2].r = r >> 1;
        p->v[2].g = g >> 1;
        p->v[2].b = b >> 1;
        p->v[2].a = 0x80;
        p->v[3].r = r;
        p->v[3].g = g;
        p->v[3].b = b;
        p->v[3].a = 0x80;
        p->v[0].x = x;
        p->v[0].y = y;
        p->v[1].x = x;
        p->v[1].y = y + 8;
        p->v[2].x = x + w;
        p->v[2].y = y;
        p->v[3].x = x + w;
        p->v[3].y = y + 8;
    }
    p->z = 0;
    p++;
    p->type = 0;
end:
    return p;
}
