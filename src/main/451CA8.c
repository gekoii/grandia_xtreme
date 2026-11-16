/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n07.h"

typedef struct {
    f32 a;
    f32 b;
} Pair;

extern Pair gphWipeSlideTable[];

void GphWipeSlide(GphWipe *o) {
    f32 s;
    f32 v;
    f32 x;
    f32 y;
    s32 off;
    f32 a;
    f32 b;

    y = 0.0f;
    s = o->frame;
    v = s * 20.0f;
    off = (((o->mode << 1) & 2) + (o->dir & 1)) * 8;
    a = *(f32 *)(off + (u8 *)gphWipeSlideTable);
    b = *(f32 *)((u8 *)gphWipeSlideTable + off + 4);
    if (v >= 640.0f) {
        v = 640.0f;
        o->state = 2;
    }
    if (s == y) {
        x = b;
    } else {
        x = b + v * a;
    }
    /* volatile: these two stores stay first, in this order (sched) */
    *(volatile u32 *)&gphWipeSprites[0].type = 1;
    *(volatile u8 *)&gphWipeSprites[0].v[0].a = 0x80;
    gphWipeSprites[0].v[0].x = x;
    gphWipeSprites[0].v[0].y = y;
    gphWipeSprites[0].v[1].x = x + 640.0f;
    gphWipeSprites[0].v[1].y = y + 448.0f;
    gphWipeSprites[0].v[0].r = 0;
    gphWipeSprites[0].v[0].g = 0;
    gphWipeSprites[0].v[0].b = 0;
    gphWipeSprites[0].z = 0;
    gphWipeSprites[1].type = 0;
}
