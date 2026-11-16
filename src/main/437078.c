#include "common.h"
#include "gph.h"

GphPart *ChrModelGetPart(GphModel *m, s32 arg1) {
    GphPart *parts;

    parts = m->parts;
    if ((parts == NULL) || (arg1 >= (s32)m->partCount)) {
        return NULL;
    }
    return &parts[arg1];
}
