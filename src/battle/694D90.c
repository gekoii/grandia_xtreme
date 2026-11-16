#include "common.h"
#include "battle/n29.h"
#include "protos/battle.h"

void BtlCmdListCellColorSet(SprColorQuad *p, s32 a, s32 b, f32 s) {
    s32 h = a + (b << 10);
    u32 c;

    c = UtilHsvToRgb(h, s, 1.0f);
    p->col[1].w = c;
    p->col[0].w = c;
    c = UtilHsvToRgb(h, 0.8f, 1.0f);
    p->col[3].w = c;
    p->col[0].b[0] = 0;
    p->col[2].w = c;
    p->col[3].b[0] = 0;
}
