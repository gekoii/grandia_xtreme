/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "battle/misc.h"

extern s16 ptclBtlDustSprite[];


void PtclBtlDustDraw(void) {
    PtclBtlDust *p;
    s32 i;

    GphTexBind(0x33);
    SprBegin(0.0f, 0.0f);
    p = ptclBtlDustPool;
    for (i = 127; i >= 0; i--, p++) {
        if (!(p->unk0 <= 0.0f)) {
            sprState.angle = p->unkE;
            sprState.col[0].r = 0x78;
            sprState.col[0].g = 0x69;
            sprState.col[0].a = p->unk0 * 128.0f;
            sprState.col[0].b = 0x4B;
            sprState.sy = p->unk4;
            sprState.sx = p->unk4;
            SprDrawRect3D(ptclBtlDustSprite, -1, -1, &p->unk10);
        }
    }
    SprFlush();
}
