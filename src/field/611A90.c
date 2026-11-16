#include "common.h"

typedef struct {
    u8 type;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 pad4[4];
    s16 h8;
    u8 padA[2];
    u8 bC;
    u8 bD;
    s16 hE;
    f32 f[6];
    u8 pad28[0x18];
    f32 f40;
    f32 f44;
} Obj;

typedef struct {
    u8 pad0[0xDC];
    u16 n;
    u8 padDE[0x13C - 0xDE];
    Obj *obj;
} Area;

void AmapAreaAdd(Area *p, s32 flags, s32 a, s32 b, s32 c, s32 d, s32 e, s32 g, s32 h) {
    Obj *o;
    s32 big;
    s32 d2;
    s32 e2;
    s32 k;
    s32 t6;
    s32 t8;
    s32 t5;

    big = flags & 0x80;
    d2 = 0;
    e2 = 0;
    o = p->obj;
    if (big) {
        d2 = d;
        e2 = e;
        o[p->n].type = 2;
    } else {
        o[p->n].type = 0;
    }
    k = flags & 0x7F;
    t8 = 0;
    t5 = 0;
    t6 = 0;
    o[p->n].b3 = 0;
    o[p->n].bC = 0;
    switch (k) {
    case 0:
        t6 = 2;
        t8 = 0;
        t5 = 1;
        break;
    case 1:
        t6 = 0;
        t8 = 3;
        t5 = 1;
        break;
    case 2:
        t6 = 0;
        t8 = 0;
        t5 = 1;
        break;
    case 3:
        t6 = 0;
        t8 = 1;
        t5 = 1;
        break;
    case 4:
        t6 = 0;
        t8 = 1;
        t5 = 3;
        break;
    case 5:
        t6 = 0;
        t8 = 0;
        t5 = 3;
        break;
    case 6:
        t6 = 0;
        t8 = 4;
        t5 = 3;
        break;
    case 9:
        t5 = 1;
        t6 = 2;
        t8 = 0;
        o[p->n].b3 = 1;
        break;
    case 10:
        t6 = 1;
        t8 = 4;
        t5 = 1;
        break;
    case 11:
        t5 = 1;
        t6 = 3;
        t8 = 4;
        o[p->n].bD = 1;
        o[p->n].hE = 1;
        break;
    }
    if (big) {
        o[p->n].b1 = t6;
        o[p->n].h8 = t5;
        o[p->n].b2 = t8;
        o[p->n].f40 = e2;
        o[p->n].f44 = d2;
        o[p->n].f[0] = a - 250;
        o[p->n].f[1] = b;
        o[p->n].f[2] = -(f32)(c - 250);
    } else {
        o[p->n].b1 = t6;
        o[p->n].h8 = t5;
        o[p->n].b2 = t8;
        o[p->n].f[0] = a - 250;
        o[p->n].f[1] = b;
        o[p->n].f[2] = -(f32)(c - 250);
        o[p->n].f[3] = d - 250;
        o[p->n].f[4] = e;
        o[p->n].f[5] = -(f32)(g - 250);
    }
    if (h != -1) {
        o[p->n].bD = 1;
        o[p->n].hE = h;
    }
    p->n++;
}
