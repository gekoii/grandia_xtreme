#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"

f32 VuSinAngle(s32);

void BtlHudMarkerResistDraw(BtlHudMarker *fx) {
    VuVec4 v;
    s32 t;

    sprState.col[0].a = 0x80;
    sprState.sy = 1.0f;
    sprState.sx = 1.0f;
    sprState.angle = 0;
    BtlChrGetPartPos(&v, fx->unit, 1);
    UtilWorldToScreen(&v, &v);
    sprState.ox = v.x;
    sprState.oy = v.y;
    t = fx->timer - 8;
    if (t < 0x10) {
        sprState.col[0].a = 0x20;
        sprState.angle = 83968.0f - VuSinAngle(t << 10) * 22528.0f;
        SprDrawRect(&btlHudMarkerRects[3], -btlHudMarkerRects[3].w / 2, -btlHudMarkerRects[3].h / 2);
    }
    t = fx->timer - 4;
    if (t < 0x10) {
        sprState.col[0].a = 0x40;
        sprState.angle = 83968.0f - VuSinAngle(t << 10) * 22528.0f;
        SprDrawRect(&btlHudMarkerRects[3], -btlHudMarkerRects[3].w / 2, -btlHudMarkerRects[3].h / 2);
    }
    t = fx->timer;
    if (t < 0x10) {
        sprState.angle = 83968.0f - VuSinAngle(t << 10) * 22528.0f;
    } else {
        sprState.angle = 0xF000;
    }
    sprState.col[0].a = 0x80;
    SprDrawRect(&btlHudMarkerRects[3], -btlHudMarkerRects[3].w / 2, -btlHudMarkerRects[3].h / 2);
}
