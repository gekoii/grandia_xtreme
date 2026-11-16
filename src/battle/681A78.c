#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void BtlFadeDraw(void) {
    BtlFade *f = &btlFade;

    if (f->quad.col[0].c.r != 0) {
        SprBegin(0.0f, 0.0f);
        SprDrawColorQuad(&f->quad, 1.0f);
        SprFlush();
    }
}
