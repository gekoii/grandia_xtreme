#include "common.h"
#include "protos/main.h"
#include "gph.h"


void ChrModelSetPartAttr(GphModel *arg0, s32 arg1, s32 arg2) {
    GphPart *p = arg0->parts;
    GphPart *end = p + arg0->partCount;

    do {
        ChrPartSetAttr(p++, arg1, arg2);
    } while (p < end);
}
