#include "common.h"
#include "pad.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertVibrationUpdate(void) {
    u32 i;
    MiniConcertVib *e;

    if (miniConcertWork->unk2E != 0) {
        miniConcertWork->unk2E--;
        return;
    }
    for (i = 0; i < miniConcertWork->unk2C; i++) {
        if (miniConcertWork->frame == miniConcertWork->vibs[i].id) {
            e = &miniConcertWork->vibs[i];
            PadVibrate(padPort0, e->a, e->b, e->c);
            miniConcertWork->unk2E = miniConcertWork->vibs[i].c;
            return;
        }
    }
}
