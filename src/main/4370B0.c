#include "common.h"
#include "gph.h"

GphPart *ChrModelSearchPart(GphModel *arg0, s32 arg1) {
    GphPart *p;
    u16 n;

    p = arg0->parts;
    if (p == NULL) {
        return NULL;
    }
    n = arg0->partCount;
    while (n != 0) {
        if (p->id == arg1) {
            return p;
        }
        p++;
    }
    return NULL;
}
