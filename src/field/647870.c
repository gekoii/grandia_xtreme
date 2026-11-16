#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertNoteHit(void) {
    u32 i;
    u32 best;
    u32 bi;
    MiniConcertNote *e;
    MiniConcertScore *sc;
    u32 n;
    s32 d;
    u8 t;
    u8 v;

    if (MiniGetUi()->input.buttonWait != 5) {
        return;
    }
    best = 0;
    bi = 0;
    for (i = 0; i < 16; i++) {
        e = &miniConcertFx->notes[i];
        if (e->unk74 != 0 && e->unk76 == 0 && best < e->unk70) {
            best = e->unk70;
            bi = i;
        }
    }
    e = &miniConcertFx->notes[bi];
    if (e->unk74 == 0 || e->unk76 != 0) {
        return;
    }
    sc = &miniConcertWork->unk50;
    sc->unk6 = 1;
    e->unk76 = 1;
    e->unk77 = 0;
    e->unk7A = 0;
    d = e->unk70 - miniConcertWork->unk16;
    n = (d < 0) ? -d : d;
    if (n <= 2) {
        n = 0;
    }
    if (n >= 3) {
        n = 1;
    }
    t = e->unk73;
    if (t == 1) {
        sc->unk8 = t;
        v = miniConcertUi->input.button;
        if (v == t) {
            if (sc->unk7 == 0) {
                sc->unk5 = 2;
            } else {
                sc->unk5 = v;
            }
        } else {
            sc->unk5 = 3;
        }
        sc->unk7 = 1;
        if (sc->unk5 == 3) {
            n = 2;
        }
        e->spr.fxFlags = 0;
        e->spr.moveCount = 0;
        e->spr.animNo = 0x12;
        e->spr.drawFlags = 2;
        e->spr.sx = e->unk68;
        e->spr.sy = e->unk6C;
        e->spr.moveTime = 6;
        GphAnimStateInit(&e->spr.anim);
    } else {
        sc->unk8 = 0;
        sc->unk5 = 0;
    }
    e->unk79 = n;
    sc->unk0 += miniConcertData->rules->unk14[sc->unk8 * 3 + n];
    MiniConcertComboAdd(n, e->unk60, e->unk64);
    MiniConcertSparksSpawn(miniConcertWork->song->unk16[n], e->unk60, e->unk64);
}
