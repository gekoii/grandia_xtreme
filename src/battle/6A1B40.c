#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"
#include "sys.h"

extern SprRect fxFailLabelRects[];

void FxFailLabelDraw(BtlUnit *arg0) {
    FxFailLabel *p = &fxFailLabel[arg0->id];
    VuVec4 v;
    SprRect *l;

    if (p->timer >= 0) {
        sprState.col[0].a = arg0->alpha * 128.0f;
        BtlChrGetPartPos(&v, arg0, 1);
        l = &fxFailLabelRects[(s8)p->frame];
        v.y = (v.y + arg0->params->colH) * 0.5f + p->y;
        SprDrawRectAtWorld(l, 0x20 - (s16)l->w / 2, -22, (f32 *)&v);
    }
}
