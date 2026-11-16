#include "common.h"
#include "protos/main.h"
#include "gph.h"


void ChrModelSetAlphaRaw(GphModel *arg0, f32 arg1) {
    s32 i;
    GphPart *e;

    e = arg0->parts;
    for (i = 0; i < arg0->partCount; i++) {
        ChrPartSetAlphaRaw(e, arg1);
        e++;
    }
}
