/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"

f32 VuSinAngle(s32);

void BtlHudGaugeDraw(void) {
    VuVec4 v;
    BtlHudUnitGauge *g;
    BtlUnit *u;
    s32 i;
    f32 t;
    f32 k;
    f32 sy;
    f32 sx;
    f32 x;

    g = btlHudUnitGauges;
    SprBegin(0.0f, 0.0f);
    u = btlUnits;
    for (i = 0; i < 0x10; i++, u++, g++) {
        if (g->hits < 0 || !(0.0f < u->alpha)) {
            continue;
        }
        BtlChrGetPartPos(&v, u, 1);
        UtilWorldToScreen(&v, &v);
        sprState.col[0].a = u->alpha * 128.0f;
        sprState.ox = v.x + -41.0f;
        sprState.oy = v.y + 48.0f;
        if (g->damage > 0 || g->timeout > 0) {
            sprState.sy = 1.0f;
            sprState.sx = 1.0f;
            BtlHudHpBarDraw(u);
        }
        if (g->hits >= 2) {
            t = 1.0f - g->popIn;
            sprState.oy -= g->scale * 16.0f;
            sprState.sx = (t * t * 4.0f + 1.0f) * g->scale;
            sy = g->popIn * (VuSinAngle(g->shake << 12) + 1.0f) * g->scale;
            sprState.sy = sy;
            MenuNumberDraw(&btlHudGaugeHitsNumFmt, g->hits);
            sprState.sy = g->popIn * g->scale;
            x = MenuNumberWidth(&btlHudGaugeHitsNumFmt, g->hits) * (1.0f - btlHudGaugeHitsNumFmt.sx) + btlHudGaugeHitsNumFmt.x;
            SprDrawRect(&btlHudHitsRect, x, 0);
            sx = sprState.sx;
            x *= sx;
            if (0.0f < g->flash) {
                k = g->flash * g->flash + 1.0f;
                sprState.col[0].a = g->flash * 64.0f;
                sprState.sx = sx * k;
                sprState.sy = sy * k;
                MenuNumberDraw(&btlHudGaugeHitsNumFmt, g->hits);
                sprState.col[0].a = g->flash * 32.0f;
                sprState.sx += sprState.sx * g->flash * g->flash;
                sprState.sy += sprState.sy * g->flash * g->flash;
                MenuNumberDraw(&btlHudGaugeHitsNumFmt, g->hits);
            }
            sprState.sx = 1.0f;
            sprState.sy = 1.0f;
            sprState.col[0].a = u->alpha * 128.0f;
            sprState.ox += x + 20.0f;
            MenuNumberDraw(&btlHudGaugeBonusNumFmt, g->hits * 3);
            SprDrawRect(&btlHudGaugeRects[12], -btlHudGaugeRects[12].w, -btlHudGaugeRects[12].h);
            SprDrawRect(&btlHudGaugeRects[11], MenuNumberWidth(&btlHudGaugeBonusNumFmt, g->hits * 3), -btlHudGaugeRects[11].h);
        }
    }
    SprFlush();
}
