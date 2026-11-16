#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "party.h"
#include "sys.h"
#include "protos/battle.h"

f32 VuSinAngle(s32);

void FxStatusIconDraw(BtlUnit *arg0, s32 arg1, VuVec4 *arg2) {
    FxStatusIcon *g = &fxStatusIcons[arg0->id][arg1];
    f32 s;

    if (g->state != 0) {
        s = g->scale * 2.0f;
        sprState.sx = s;
        sprState.sy = s;
        sprState.col[0].a = g->alpha * 128.0f * arg0->alpha * fxStatusIconAlpha[arg0->id];
        arg2->y = arg0->pos[1] + arg0->params->colH + g->pos +
                  VuSinAngle((btlTickCount << 10) - arg1 * 0x3000) * 8.0f;
        SprDrawRectAtWorld(&menuStatusIcons[arg1], -12, -24, (f32 *)arg2);
    }
}
