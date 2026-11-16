#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/n25.h"

extern u8 miniSparkleRect[];

void MiniSparklesDraw(void) {
    MiniSparkle *it;
    s32 i;
    u8 f;
    f32 h;
    s32 t;
    u8 c;
    u8 m;
    f32 x, y;
    s32 v;

    i = -1;
    if (miniSparkleIndex == i) {
        return;
    }
    GphTexBind(0xEC);
    m = 0x80;
    it = miniSparkles;
    miniUi->curTex = i;
    SprBegin(0.0f, 0.0f);
    c = 0x60;
    h = 0.5f;
    t = 2;
    sprState.col[0].b = sprState.col[0].g = c;
    sprState.blend = t;
    sprState.sx = sprState.sy = h;
    for (i = 31; i >= 0; i--, it++) {
        f = it->timer;
        if (f != 0) {
            x = it->x;
            v = (u16)it->angle;
            y = it->y;
            sprState.ox = x;
            sprState.oy = y;
            sprState.angle = v;
            if ((s8)f >= 0) {
                sprState.col[0].a = f;
            } else {
                sprState.col[0].a = m;
            }
            SprDrawRect(miniSparkleRect, -0xC, -0xC);
        }
    }
    SprFlush();
}
