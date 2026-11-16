#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxStatusIconsUpdate(BtlUnit *arg0) {
    u32 bit;
    s32 flags;
    FxStatusIcon *g;
    f32 y;

    flags = arg0->params->status & ~0xA000;
    g = fxStatusIcons[arg0->id];
    for (bit = 0x10; bit <= 0x7FFF; bit <<= 1, g++) {
        FxStatusIconSetActive(g, flags & bit);
    }
    y = 0.0f;
    FxStatusIconMove(&fxStatusIcons[arg0->id][0], y);
    FxStatusIconMove(&fxStatusIcons[arg0->id][7], y);
    FxStatusIconMove(&fxStatusIcons[arg0->id][10], y);
    if (fxStatusIcons[arg0->id][0].state > 0 || fxStatusIcons[arg0->id][7].state > 0 ||
        fxStatusIcons[arg0->id][10].state > 0) {
        y += 40.0f;
    }
    FxStatusIconMove(&fxStatusIcons[arg0->id][1], y);
    FxStatusIconMove(&fxStatusIcons[arg0->id][2], y);
    FxStatusIconMove(&fxStatusIcons[arg0->id][3], y);
    if (fxStatusIcons[arg0->id][1].state > 0 || fxStatusIcons[arg0->id][2].state > 0 ||
        fxStatusIcons[arg0->id][3].state > 0) {
        y += 40.0f;
    }
    FxStatusIconMove(&fxStatusIcons[arg0->id][4], y);
    if (fxStatusIcons[arg0->id][4].state > 0) {
        y += 40.0f;
    }
    FxStatusIconMove(&fxStatusIcons[arg0->id][5], y);
    if (fxStatusIcons[arg0->id][5].state > 0) {
        y += 40.0f;
    }
    FxStatusIconMove(&fxStatusIcons[arg0->id][6], y);
    if (fxStatusIcons[arg0->id][6].state > 0) {
        y += 40.0f;
    }
    FxStatusIconMove(&fxStatusIcons[arg0->id][8], y);
    if (fxStatusIcons[arg0->id][8].state > 0) {
        y += 40.0f;
    }
}
