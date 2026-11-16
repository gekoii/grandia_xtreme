#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n25.h"

void MemSet(void *, s32, s32);

void MiniBallStart(void) {
    MemSet(miniBallWork, 0, 0x1FD0);
    miniBallWork->state = 1;
    miniBallWork->subState = 0;
    miniBallWork->game.ballHitVol = 0xFF;
    miniBallWork->game.wallHitVol = 0xFF;
    GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 70.0f, 10.0f);
    GphScreenFxSet(fldScreenFx, 2, 1, 0x14, 3.0f);
}
