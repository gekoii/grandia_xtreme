#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"

extern SprRect menuSparkleSprite[];

void MenuSparkleDraw(void) {
    MenuSparkle *it;
    s32 i;
    u8 f;
    f32 h;
    s32 t;
    u8 c;
    f32 x, y;
    s32 v;

    it = menuSparkles;
    SprBegin(0.0f, 0.0f);
    h = 0.5f;
    c = 0x60;
    t = 2;
    /* volatile: keeps these sprState stores in place and in order; plain stores
     * schedule differently (in the loop, 0x80 moves into a saved register) */
    (*(volatile SprState *)&sprState).col[0].g = c;
    (*(volatile SprState *)&sprState).sx = h;
    (*(volatile SprState *)&sprState).blend = t;
    (*(volatile SprState *)&sprState).sy = h;
    (*(volatile SprState *)&sprState).col[0].b = c;
    for (i = 0; i < 32; i++, it++) {
        f = it->timer;
        if (f != 0) {
            x = it->x;
            v = it->angle;
            y = it->y;
            (*(volatile SprState *)&sprState).ox = x;
            (*(volatile SprState *)&sprState).oy = y;
            sprState.angle = v;
            (*(volatile SprState *)&sprState).col[0].a = ((s8)f < 0) ? 0x80 : f;
            SprDrawRect(menuSparkleSprite, -0xC, -0xC);
        }
    }
    SprFlush();
}
