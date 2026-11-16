#include "common.h"
#include "field/n19.h"

void MemSet(void *, s32, s32);

void FldLightFadeInit(void) {
    MemSet(&fldLightFade, 0, sizeof(FldLightFade));
}
