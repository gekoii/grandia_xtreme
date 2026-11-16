/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/n24.h"


void MiniConcertSparksDraw(void) {
    MiniPanel *lk = &miniConcertWork->panels[1];
    u32 i;
    MiniConcertSpark *b;
    f32 a;
    SprRect *r;

    if (miniConcertUi->curTex != lk->tex) {
        GphTexReplace((GphTex *)((GphAnimSet **)miniConcertLayout->images)[lk->tex], 0xBE);
        GphTexBind(0xBE);
        miniConcertUi->curTex = lk->tex;
    }
    for (i = 0; i < 128; i++) {
        b = &miniConcertFx->sparks[i];
        a = 1.0f;
        if (b->unk1C != 0) {
            if (b->unk1C == 4) {
                a = 1.0f - (f32)b->unk1D / (f32)b->unk1A;
            }
            SprBegin(b->unk0, b->unk4);
            sprState.col[0].a = a * 128.0f;
            r = (SprRect *)(b->unk1E * 12 + (u32)miniConcertLayout->rects);
            SprDrawRect(r, -(r->w * 0.5f), -(r->h * 0.5f));
            SprFlush();
        }
    }
}
