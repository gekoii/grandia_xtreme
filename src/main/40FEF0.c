#include "common.h"
#include "protos/main.h"
#include "main/n01.h"


void SprDrawColorQuadTinted(SprColorQuad *q, u32 c, f32 alpha, f32 t) {
    u32 save[4];
    s32 i;
    f32 s = 1.0f - t;
    u8 *cp;
    u8 *p;
    u8 r, g, b;

    for (i = 0; i < 4; i++) {
        save[i] = q->col[i].word;
    }
    cp = (u8 *)&c;
    b = cp[3];
    g = cp[2];
    r = cp[1];
    p = (u8 *)q;
    for (i = 0; i < 4; i++) {
        p[0xB] = (p[0xB] - b) * s + b;
        p[0xA] = (p[0xA] - g) * s + g;
        p[0x9] = (p[0x9] - r) * s + r;
        p += 4;
    }
    SprDrawColorQuad(q, alpha);
    for (i = 0; i < 4; i++) {
        q->col[i].word = save[i];
    }
}
