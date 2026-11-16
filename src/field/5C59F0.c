#include "common.h"
#include "field/n14.h"

#define SET_SPR(p, U0, V0, W, H, X0, Y0)            \
    {                                              \
        p->type = 1;                               \
        p->v[0].r = 0x80;                          \
        p->v[0].g = 0x80;                          \
        p->v[0].b = 0x80;                          \
        p->v[0].a = 0x80;                          \
        p->v[0].x = X0;                            \
        p->v[0].y = Y0;                            \
        p->v[1].x = X0 + W;                        \
        p->v[1].y = Y0 + H;                        \
        p->z = 0;                                  \
        p->v[0].u = U0;                            \
        p->v[0].v = V0;                            \
        p->v[1].u = U0 + W;                        \
        p->v[1].v = V0 + H;                        \
        p++;                                       \
    }

GphBatchQuad *FldHpBarBuildSprites(GphBatchQuad *p, s32 w0, s32 mid, s32 w2, s32 x, s32 y) {
    s32 w;

    SET_SPR(p, 0, 0xB8, w0, 26, x, y);
    if (mid != 0) {
        w = mid - w0;
        SET_SPR(p, w0, 0xD0, w, 26, x + w0, y);
    }
    SET_SPR(p, w2, 0xE8, 0x54 - w2, 26, x + w2, y);
    p->type = 0;
    return p;
}
