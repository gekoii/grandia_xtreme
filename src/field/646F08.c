/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "gph.h"
#include "field/n24.h"

typedef struct {
    f32 x, y, z, w;
} Vec;

void VuVecCopy(Vec *, Vec *);

void MiniConcertPlayInit(void) {
    MiniConcertWork *w;
    MiniConcertSong *sub;
    MiniConcertActor *s;
    ChrObj *obj;
    u32 i, j;
    u8 k;
    u8 tex;
    u32 base;
    MiniConcertWork *w2;
    MiniSprite *e;
    MiniPanel *b1, *b2;

    w = miniConcertWork;
    sub = miniConcertData->songs[w->songNo];
    w->song = sub;
    w->unk4 = sub->unk4 / 100.0f;
    w->unk8 = sub->unk6;
    w->unkC = sub->unk8;
    w->unk10 = sub->unk0;
    w->unk14 = sub->unkA;
    w->unk16 = sub->unkC;
    w->unk18 = sub->unkE;
    w->unk1A = sub->unk10;
    w->unk1C = sub->unk12;
    w->unk1E = sub->unk14;
    w->unk20 = sub->unk20;
    w->unk2C = sub->unk22;
    w->spots = miniConcertData->spots[w->songNo];
    w->vibs = miniConcertData->vibs[w->songNo];
    w->unk22 = w->songNo;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            miniConcertWork->carros[i][j]->flags &= ~1;
        }
    }

    for (i = 0; i < 5; i++) {
        s = (MiniConcertActor *)((u8 *)miniConcertWork + (i * 0x20 + 0x89F4));
        if (s->unk1D == -1) {
            s->state = 0;
            continue;
        }
        tex = 0;
        if (miniConcertWork->stage.unk18FB == -1) {
            k = miniConcertWork->unk22;
        } else {
            k = miniConcertWork->stage.unk18FB + 5;
        }
        switch (s->unk1C / 3) {
        case 0:
            tex = miniConcertData->unk20[k][0];
            break;
        case 1:
            tex = miniConcertData->unk20[k][1];
            break;
        case 2:
            tex = miniConcertData->unk20[k][2];
            break;
        }
        obj = miniConcertWork->carros[s->unk1E][s->unk1F];
        s->chr = obj;
        obj->flags = (obj->flags | 0x41) & ~0x80;
        s->state = 1;
        base = miniConcertData->unk1C + tex * 64;
        s->pat = 0xFF;
        s->sub = 0xFF;
        s->pats[0] = (MiniConcertPattern *)base;
        s->pats[1] = (MiniConcertPattern *)(base + 0x10);
        s->pats[2] = (MiniConcertPattern *)(base + 0x20);
        s->pats[3] = (MiniConcertPattern *)(base + 0x30);
        VuVecCopy((Vec *)obj->model->pos, (Vec *)miniConcertData->camPath->m[s->unk1C]);
        s->chr->moveDir = miniConcertData->camPath->ang[s->unk1C] / 360.0f * 65536.0f;
        ChrMotSet(s->chr, 1);
    }

    miniConcertWork->unk50.unk7 = 1;
    miniConcertWork->unk50.unk4 = 2;
    w2 = miniConcertWork;
    b1 = &w2->panels[1];
    b2 = &w2->panels[3];
    MiniPanelInit(w2->panels, miniConcertData->unk3C, miniConcertData->unk40, w2->sprites);
    e = &w2->sprites[1];
    for (i = 0; i < 8; i++) {
        e->ofsX = cosf(i * 0.78539753f - 1.57079506f) * 60.0f + 552.0f;
        e->ofsY = sinf(i * 0.78539753f - 1.57079506f) * 60.0f + 342.0f;
        e->angle = i << 13;
        e++;
    }
    MiniPanelInit(b1, miniConcertData->unk3C + 0x20, miniConcertData->unk40, w2->sprites);
    for (i = 0; i < 16; i++) {
        MiniSpriteInit(&miniConcertFx->notes[i].spr, miniConcertData->unk40 + 0x48, 1, 1, 1);
    }
    MiniPanelInit(b2, miniConcertData->unk3C + 0x60, miniConcertData->unk40, w2->sprites);
    for (i = 0; i < 128; i++) {
        MiniSpriteInit(&miniConcertFx->bursts[i].spr, miniConcertData->unk40 + 0x60, 10, 10, 1);
        miniConcertFx->bursts[i].spr.drawFlags = 2;
    }
}
