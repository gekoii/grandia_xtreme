#include "common.h"
#include "field/menu.h"
#include "field/n18.h"

FldMenuQuad *TxtMsgSpritesBuild(s32 arg0, FldMenuQuad *p, TxtMsgHdr *h, s32 x, s32 y, s32 w, s32 hgt, s32 idx) {
    s32 i;
    u8 *len;

    len = (u8 *)(h->lenOfs + (s32)h) + idx;
    for (i = 0; i < *len; i++) {
        p->v[0].r = 0x80;
        p->v[0].g = 0x80;
        p->v[0].b = 0x80;
        p->v[0].x = x + i * w;
        p->v[0].y = y;
        p->v[1].y = y + hgt;
        p->type = 1;
        p->z = 0;
        p->v[0].u = i * 18;
        p->v[0].v = idx * 18;
        p->v[1].v = idx * 18 + 16;
        p->v[0].a = 0x80;
        p->v[1].x = x + i * w + w;
        p->v[1].u = i * 18 + 16;
        p++;
    }
    p->type = 0;
    return p;
}
