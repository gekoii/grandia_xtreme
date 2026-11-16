#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "gph.h"
#include "field/n24.h"


void MiniConcertNotesDraw(void) {
    MiniPanel *lk = &miniConcertWork->panels[1];
    u8 last = 0;
    u32 i;
    MiniConcertNote *e;
    f32 hw;
    f32 hh;
    SprRect *r;
    u8 a;

    if (miniConcertUi->curTex != lk->tex) {
        GphTexReplace((GphTex *)((GphAnimSet **)miniConcertLayout->images)[lk->tex], 0xBE);
        GphTexBind(0xBE);
        miniConcertUi->curTex = lk->tex;
    }
    for (i = 0; i < 16; i++) {
        e = &miniConcertFx->notes[i];
        if (e->unk74 != 0) {
            SprBegin(lk->x, lk->y);
            a = e->spr.a;
            sprState.col[0].a = a;
            r = (SprRect *)(e->unk78 * 12 + (u32)miniConcertLayout->rects);
            hw = r->w * 0.5f;
            hh = r->h * 0.5f;
            if (e->unk7A == 1) {
                SprPushTransform(0, e->unk60, e->unk64, e->unk68, e->unk6C);
                SprDrawRect(&miniConcertLayout->rects[e->unk78], -hw, -hh);
                SprPopTransform();
                SprFlush();
            }
            if (e->spr.active != 0 && e->spr.animNo == 0) {
                SprPushTransform(e->spr.angle, e->spr.x + e->spr.waveX + e->spr.ofsX, e->spr.y + e->spr.waveY + e->spr.ofsY, e->spr.sx * e->unk68, e->spr.sy * e->unk6C);
                SprDrawRect(&miniConcertLayout->rects[e->spr.rect], -hw, -hh);
                SprPopTransform();
                SprFlush();
            }
        }
    }
    for (i = 0; i < 16; i++) {
        e = &miniConcertFx->notes[i];
        if (e->unk74 != 0 && e->spr.active != 0 && e->spr.animNo != 0) {
            if (e->spr.animNo != last) {
                last = e->spr.animNo;
                GphAnimSetSelect(((GphAnimSet **)miniConcertLayout->images)[last + miniConcertData->hdr->unk15 - 1]);
                GphTexReplace(((GphAnimSet **)miniConcertLayout->images)[last + miniConcertData->hdr->unk15 - 1]->tex, 0xBE);
                GphTexBind(0xBE);
                miniConcertUi->curTex = -1;
            }
            MiniSpriteDrawAnim(&miniConcertWork->panels[1], &e->spr);
        }
    }
}
