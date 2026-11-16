#include "common.h"
#include "protos/main.h"
#include "gph.h"


void ChrModelSetAlpha(GphModel *arg0, f32 arg1) {
    s32 i;
    GphPart *p;

    p = arg0->parts;
    for (i = 0; i < arg0->partCount; i++) {
        ChrPartSetAlpha(p, arg1);
        p++;
    }
}
