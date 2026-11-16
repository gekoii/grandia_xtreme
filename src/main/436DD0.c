#include "common.h"
#include "gph.h"

void ChrModelUvScrollUpdate(GphModel *m, f32 f) {
    GphPart *p;
    s32 n;

    p = m->parts;
    n = m->partCount;

    do {
        n--;
        if (p->attr & 0x40) {
            p->uvScroll[0] = (s32)(p->uvScroll[0] + p->uvScroll[1] * f) & 0xFFF;
            p->uvScroll[2] = (s32)(p->uvScroll[2] + p->uvScroll[3] * f) & 0xFFF;
        }
        p++;
    } while (n != 0);
}
