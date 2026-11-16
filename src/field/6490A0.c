#include "common.h"
#include "field/n24.h"

void MiniConcertComboBreak(void) {
    u32 i;
    MiniConcertScore *sc = &miniConcertWork->unk50;
    MiniConcertComboMark *r;

    sc->unk4 = 2;
    sc->unk2 = 0;
    for (i = 0; i < 128; i++) {
        r = &miniConcertFx->marks[i];
        if (r->unk7C != 0 && r->unk7C != 4) {
            r->unk7C = 4;
            r->unk78 = 0;
            r->unk76 = 90;
            r->unk70 = r->unk60;
            r->unk72 = r->unk64;
        }
    }
}
