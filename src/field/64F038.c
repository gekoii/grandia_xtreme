#include "common.h"
#include "field/n25.h"
#include "protos/field.h"

void MiniSparkleBurst(MiniPanel *h, MiniSprite *o, s32 rnd, s32 base) {
    u32 i;
    u32 n;
    f32 x;
    f32 y;

    n = VuRandInt(rnd) + base;
    for (i = 0; i < n; i++) {
        x = o->x + o->ofsX;
        y = o->y + o->ofsY;
        x = h->x + x * h->sx;
        y = h->y + y * h->sy;
        if (o->topLeft == 0) {
            x += o->cellW * 0.5f;
            y += o->cellH * 0.5f;
        }
        MiniSparkleSpawn(x, y);
    }
}
