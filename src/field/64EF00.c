#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

typedef struct {
    u8 b[0x40];
} Spr;


void MiniSpriteDrawNumber(MiniPanel *h, MiniSprite *o) {
    Spr buf[0x40];
    f32 x;
    f32 y;
    u8 a;
    f32 sx;
    f32 sy;

    x = o->x + (o->ofsX + (o->waveX + o->numOfsX));
    y = o->y + (o->ofsY + (o->waveY + o->numOfsY));
    x = h->x + x * h->sx;
    y = h->y + y * h->sy;
    sx = o->num->w * o->sx * (1.0f / 22.0f);
    sy = o->num->h * o->sy * (1.0f / 22.0f);
    a = o->a * (o->alphaScale * (1.0f / 128.0f)) * (h->alpha * (1.0f / 128.0f));
    ((Spr *(*)(Spr *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(buf, o->num->value, o->num->align, x, y, a, sx, sy);
    GphBatchDrawSprites(buf);
}
