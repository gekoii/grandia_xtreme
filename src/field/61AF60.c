#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

typedef union {
    double d;
    struct {
        s32 lo;
        s32 hi;
    } s;
} DU; /* a local union with a double member reserves the frame's unused 16 bytes without blocking the tail call */

/* indexed by hand: &evtFaceWins[i] computes the address differently (16 words) */
#define W(i) ((EvtFaceWin *)((i) * sizeof(EvtFaceWin) + (u32)evtFaceWins))

void EvtFaceWinOpen(u8 idx, u16 id, s16 x, s16 y, s16 w, s16 h, u16 z) {
    EvtFaceWin *e;
    GphBatchQuad *sp;
    s32 len;
    s32 t;
    DU u;
    EvtFaceWin *e2;

    if (idx >= 3) {
        return;
    }
    if (id == 0) {
        EvtFaceWinClose(idx);
        return;
    }
    if (W(idx)->active == 1 && W(idx)->id == id && W(idx)->x == x && W(idx)->y == y && W(idx)->w == w && W(idx)->h == h && W(idx)->z == z) {
        return;
    }
    W(idx)->y = y;
    W(idx)->w = w;
    W(idx)->h = h;
    W(idx)->z = z;
    W(idx)->active = 1;
    W(idx)->x = x;
    W(idx)->id = id;
    len = EvtFaceWinSetName(ChrPortraitGetName(id), idx + 10);
    e2 = &evtFaceWins[idx];
    sp = &e2->spr;
    sp->type = 1;
    sp->v[0].r = 0x80;
    sp->v[0].g = 0x80;
    sp->v[0].b = 0x80;
    sp->v[0].a = 0x80;
    e = W(idx);
    sp->z = 0;
    e2->sprEnd = 0;
    sp->v[0].x = e->x;
    sp->v[0].y = e->y;
    sp->v[1].x = e->x + e->w;
    sp->v[1].y = e->y + e->h;
    sp->v[0].u = 0;
    sp->v[0].v = 0;
    sp->v[1].u = 0x60;
    sp->v[1].v = 0x60;
    y = e->y + e->h + 3;
    w = TxtCodesGetWidth(ChrPortraitGetName(id), len, 0x18);
    h = 0x18;
    if (idx != 0) {
        x = 0x240 - w;
    } else {
        t = w / 2 - 0x30;
        x = x - t;
    }
    FldMenuWinOpen(idx + 4, x + w / 2, y + 0xF, w + 0x2C, h, 0, 9, 0x10000);
    W(idx)->nameX = x;
    W(idx)->nameY = y - 1;
    W(idx)->nameLen = len;
    u.s.lo = 0;
    W(idx)->unk14 = u.s.lo;
}
