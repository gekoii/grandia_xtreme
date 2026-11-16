#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void FldSndInit(void) {
    FldSndEmitter **p;
    void *t;

    p = &fldSndEmitters;
    t = FldMemAlloc(0x1000);
    *p = t;
    MemSet(t, 0, 0x1000);
    fldSndEmitterCount = 0;
    SndSetDistanceScale(6000.0f);
}
