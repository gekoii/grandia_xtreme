#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n24.h"


void MiniConcertComboMarksDraw(void) {
    u32 i;
    s32 c10 = 0x10;
    s32 c11 = 0x11;
    MiniConcertComboMark *r;

    if (miniConcertFx->unk9800 == 1) {
        GphAnimSetSelect(((GphAnimSet **)miniConcertLayout->images)[miniConcertData->hdr->unk15 + c10 - 1]);
        GphTexReplace(((GphAnimSet **)miniConcertLayout->images)[miniConcertData->hdr->unk15 + c10 - 1]->tex, 0xBE);
        GphTexBind(0xBE);
        miniConcertUi->curTex = -1;
    }
    for (i = 0; i < 128; i++) {
        r = &miniConcertFx->marks[i];
        if (r->unk7C != 0 && r->spr.animNo == 0x10) {
            MiniSpriteDrawAnim(miniConcertWork->panels, &r->spr);
        }
    }
    if (miniConcertFx->unk9801 == 1) {
        GphAnimSetSelect(((GphAnimSet **)miniConcertLayout->images)[miniConcertData->hdr->unk15 + c11 - 1]);
        GphTexReplace(((GphAnimSet **)miniConcertLayout->images)[miniConcertData->hdr->unk15 + c11 - 1]->tex, 0xBE);
        GphTexBind(0xBE);
        miniConcertUi->curTex = -1;
    }
    for (i = 0; i < 128; i++) {
        r = &miniConcertFx->marks[i];
        if (r->unk7C != 0 && r->spr.animNo == 0x11) {
            MiniSpriteDrawAnim(miniConcertWork->panels, &r->spr);
        }
    }
}
