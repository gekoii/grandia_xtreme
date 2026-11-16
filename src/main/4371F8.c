#include "common.h"
#include "gph.h"
#include "main/n05.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrModelCopyPartAttr(GphModel *m, ChrModelData *src) {
    s32 i = 0;
    s32 n;
    GphPart *d;
    GphPart *s;
    GphGaObject *dj;
    GphGaObject *sj;

    if (m->partCount == src->partCount) {
        s = src->parts;
        d = m->parts;
        for (; i < m->partCount; i++) {
            d->attr = s->attr;
            d->unk52 = s->unk52;
            d->gsFlags = s->gsFlags;
            d->alphaFix = s->alphaFix;
            d->unk59 = s->unk59;
            d->uvScroll[0] = s->uvScroll[0];
            d->uvScroll[1] = s->uvScroll[1];
            d->uvScroll[2] = s->uvScroll[2];
            d->uvScroll[3] = s->uvScroll[3];
            copyq(d->color, s->color);
            n = d->gaObjCount;
            dj = d->gaObjs;
            sj = s->gaObjs;
            for (; n > 0; n--) {
                dj->mode = sj->mode;
                dj->unk30 = sj->unk30;
                sj++;
                dj++;
            }
            d++;
            s++;
        }
    }
}
