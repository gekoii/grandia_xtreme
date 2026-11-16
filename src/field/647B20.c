#include "common.h"
#include "field/n24.h"

void MiniConcertNoteMiss(MiniConcertNote *e) {
    u8 t = e->unk73;
    MiniConcertScore *sc = &miniConcertWork->unk50;

    if (t == 1) {
        sc->unk5 = 3;
        sc->unk6 = t;
        sc->unk8 = t;
        sc->unk7 = t;
    } else {
        sc->unk8 = 0;
        sc->unk5 = 0;
        sc->unk7 = 0;
    }
}
