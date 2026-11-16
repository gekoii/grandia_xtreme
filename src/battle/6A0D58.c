#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "sys.h"

extern SprRect fxCastSparkRect;

void FxCastSparkDraw(void) {
    FxCastSpark *p = fxCastSparks;
    s32 i;

    GphTexBind(0x34);
    SprBegin(0.0f, 0.0f);
    sprState.blend = 2;
    SprSetColor(50, 70, 160);
    for (i = 0; i < 50; i++, p++) {
        if (p->life > 0) {
            f32 f = p->scale * p->alpha;
            sprState.sy = f;
            sprState.sx = f;
            SprDrawRect3D(&fxCastSparkRect, -5, -5, &p->pos);
        }
    }
    SprFlush();
}
