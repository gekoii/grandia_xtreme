#include "common.h"
#include "field/n14.h"

extern void *FldMemAlloc(s32);

s32 MapPathInit(void) {
    s32 i;
    s16 *p;
    MapPathWork **a = &mapPathWork;
    f32 **b = &mapSplineTriUpper;
    f32 **c = &mapSplineTriDiag;
    f32 **d = &mapSplineStepLengths;

    *a = FldMemAlloc(0xFF30);
    mapSplineSecondDeriv[0] = FldMemAlloc(0x54);
    mapSplineSecondDeriv[1] = FldMemAlloc(0x54);
    mapSplineSecondDeriv[2] = FldMemAlloc(0x54);
    *b = FldMemAlloc(0x54);
    *c = FldMemAlloc(0x54);
    *d = FldMemAlloc(0xAA00);
    p = (*a)->bigUsed;
    for (i = 7; i >= 0; i--) {
        p[-8] = 0;
        p[0] = 0;
        p++;
    }
    mapPathCtrlCount = 0;
    mapPathPointCount = 0;
    return 0;
}
