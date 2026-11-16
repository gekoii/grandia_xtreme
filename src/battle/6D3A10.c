#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void BtlHudMarkerFailDraw(BtlHudMarker *fx) {
    VuVec4 v;
    f32 c;
    f32 s;
    f32 t;
    f32 hh;
    f32 hw;
    SprRect *rc;

    sprState.col[0].a = 0x80;
    sprState.sx = 1.0f;
    sprState.sy = 1.0f;
    sprState.angle = 0;
    BtlChrGetPartPos(&v, fx->unit, 1);
    UtilWorldToScreen(&v, &v);
    sprState.ox = v.x;
    sprState.oy = v.y;
    t = 1.0f - VuCosAngle(fx->timer << 8);
    VuSinCosAngle(&c, &s, t * t * 81920.0f + 8192.0f);
    rc = &btlHudMarkerRects[7];
    t = fx->timer * 1.25f;
    c *= t;
    s *= t;
    hw = btlHudMarkerRects[7].w / 2;
    hh = btlHudMarkerRects[7].h / 2;
    sprState.col[0].a = fx->timer * 0.015625f * 80.0f;
    SprDrawRect(rc, c - hw, s - hh);
    SprDrawRect(rc, s - hw, -c - hh);
    SprDrawRect(rc, -c - hw, -s - hh);
    SprDrawRect(rc, -s - hw, c - hh);
}
