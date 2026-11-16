#include "common.h"
#include "gph.h"

void ChrModelUvScrollReset(GphModel *m) {
    s32 i;
    GphPart *p = m->parts;

    for (i = m->partCount; i > 0; i--) {
        p->uvScroll[0] = 0;
        p->uvScroll[2] = 0;
        p++;
    }
}
