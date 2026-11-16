#include "common.h"
#include "gph.h"
#include "field/n23.h"

void FldCollPartGatherSphere(GphPart *, s32, f32);

s32 FldCollModelGatherSphere(GphModel *arg0, s32 arg1, f32 arg3) {
    s32 i;
    GphPart *e;
    GphPart *elems;

    fldCollPartIndex = 0;
    elems = arg0->parts;
    for (i = 0; i < arg0->partCount; i++) {
        e = &elems[i];
        if (e->gaObjCount != 0) {
            FldCollPartGatherSphere(e, arg1, arg3);
        }
        fldCollPartIndex++;
    }
    return 1;
}
