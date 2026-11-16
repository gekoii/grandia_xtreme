#include "common.h"
#include "battle/n28.h"

void BtlFadeSetColor(s32 c) {
    BtlFade *f = &btlFade;

    f->t = 0;
    f->to.w = c;
    f->from.w = c;
    f->quad.col[3].w = c;
    f->quad.col[1].w = c;
    f->quad.col[2].w = c;
    f->dur = 0;
    f->quad.col[0].w = c;
}
