#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/n24.h"


void MiniConcertBurstsDraw(void) {
    MiniPanel *lk = &miniConcertWork->panels[3];
    u32 i;
    MiniConcertBurst *b;
    SprRect *r;

    if (miniConcertUi->curTex != lk->tex) {
        GphTexReplace((GphTex *)((GphAnimSet **)miniConcertLayout->images)[lk->tex], 0xBE);
        GphTexBind(0xBE);
        miniConcertUi->curTex = lk->tex;
    }
    for (i = 0; i < 128; i++) {
        b = &miniConcertFx->bursts[i];
        if (b->unk7C != 0) {
            SprBegin(b->unk60, b->unk64);
            sprState.col[0].a = b->spr.a;
            r = (SprRect *)(b->unk7E * 12 + (u32)miniConcertLayout->rects);
            SprDrawRect(r, -(r->w * 0.5f), -(r->h * 0.5f));
            SprFlush();
        }
    }
}
