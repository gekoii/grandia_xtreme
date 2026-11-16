/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"

typedef union {
    u32 w;
    struct {
        u32 a : 8;
        u32 b : 8;
        u32 g : 8;
        u32 r : 8;
    } c;
} Color;


u32 UtilHsvToRgb(s32 hue, f32 s, f32 v) {
    Color col;
    f32 h;
    f32 fl;
    f32 q;
    f32 p;
    f32 t;
    s32 i;

    if (s < 0.0f) {
        s = 0.0f;
    }
    if (s > 1.0f) {
        s = 1.0f;
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (v > 1.0f) {
        v = 1.0f;
    }
    hue &= 0xFFFF;
    h = hue / 10922.667f;
    fl = floorf(h);
    h = h - fl;
    i = fl;
    p = v * (1.0f - s);
    q = v * (1.0f - s * h);
    t = v * (1.0f - s * (1.0f - h));
    switch (i) {
    case 0:
        col.c.r = (s32)(v * 255.0f);
        col.c.g = (s32)(t * 255.0f);
        col.c.b = (s32)(p * 255.0f);
        break;
    case 1:
        col.c.r = (s32)(q * 255.0f);
        col.c.g = (s32)(v * 255.0f);
        col.c.b = (s32)(p * 255.0f);
        break;
    case 2:
        col.c.r = (s32)(p * 255.0f);
        col.c.g = (s32)(v * 255.0f);
        col.c.b = (s32)(t * 255.0f);
        break;
    case 3:
        col.c.r = (s32)(p * 255.0f);
        col.c.g = (s32)(q * 255.0f);
        col.c.b = (s32)(v * 255.0f);
        break;
    case 4:
        col.c.r = (s32)(t * 255.0f);
        col.c.g = (s32)(p * 255.0f);
        col.c.b = (s32)(v * 255.0f);
        break;
    case 5:
        col.c.r = (s32)(v * 255.0f);
        col.c.g = (s32)(p * 255.0f);
        col.c.b = (s32)(q * 255.0f);
        break;
    }
    col.c.a = 0x80;
    return col.w;
}
