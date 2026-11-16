#include "common.h"
#include "gph.h"

void BtlChrModelSetUvScroll(GphModel *m, s16 a, s16 b) {
    s32 n = m->partCount;
    GphPart *p = m->parts;

    for (; n > 0; n--) {
        p->uvScroll[1] = a;
        p->uvScroll[3] = b;
        p++;
    }
}
