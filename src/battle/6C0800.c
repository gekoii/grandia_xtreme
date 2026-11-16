#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"
#include "sys.h"

extern SprRect ipIconPartyRects[];
extern SprRect ipIconUnitLabelRects[];
extern SprRect btlUnitIconFrameRect;

void VuSinCosAngle(f32 *, f32 *, s32);

void BtlUnitIconDraw(BtlUnit *arg0, s16 x, s16 y, s16 ang) {
    f32 v[2];

    if (arg0->id < 4) {
        SprDrawRect(&ipIconPartyRects[arg0->params->icon], x - 12, y - 12);
        SprDrawRect(&btlUnitIconFrameRect, x - 16, y - 16);
        return;
    }
    SprDrawRect(&menuHelpVsTypeIcons[arg0->params->icon], x - 12, y - 12);
    SprDrawRect(&btlUnitIconFrameRect, x - 16, y - 16);
    VuSinCosAngle(&v[0], &v[1], ang);
    SprDrawRect(&ipIconUnitLabelRects[arg0->id], (s32)(x + v[0] * 16.0f - 9.0f), (s32)(y - v[1] * 16.0f - 9.0f));
}
