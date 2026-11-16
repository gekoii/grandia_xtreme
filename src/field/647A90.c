#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertLampsFlash(void) {
    u32 i;
    MiniSprite *o;

    for (i = 0; i < 4; i++) {
        o = &miniConcertWork->sprites[VuRandInt(8) + 1];
        o->active = 1;
        o->moveFlags = 8;
        o->moveCount = 0;
        o->moveTime = 30;
    }
}
