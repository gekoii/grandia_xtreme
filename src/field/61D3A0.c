#include "common.h"
#include "gph.h"
#include "field/n23.h"

void FldCollPartGatherCapsule(GphPart *, s32, s32, f32);

s32 FldCollModelGatherCapsule(GphModel *arg0, s32 arg1, s32 arg2, f32 arg3) {
    s32 i;
    GphPart *e;
    GphPart *elems;

    fldCollPartIndex = 0;
    elems = arg0->parts;
    for (i = 0; i < arg0->partCount; i++) {
        e = &elems[i];
        if (e->gaObjCount != 0) {
            FldCollPartGatherCapsule(e, arg1, arg2, arg3);
        }
        fldCollPartIndex++;
    }
    return 1;
}
