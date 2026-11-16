#include "common.h"
#include "gph.h"
#include "main/n05.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrModelCopyTransform(GphModel *arg0, ChrModelData *arg1) {
    GphPart *s;
    GphPart *d;
    s32 i;
    s32 n;

    if (arg0->partCount == arg1->partCount) {
        copyq(arg0->pos, arg1->pos);
        copyq(arg0->rot, arg1->rot);
        copyq(arg0->scale, arg1->scale);
        arg0->flags = arg1->flags;
        n = arg0->partCount;
        s = arg1->parts;
        d = arg0->parts;
        for (i = 0; i < n; i++) {
            copyq(d->pos, s->pos);
            copyq(d->rot, s->rot);
            copyq(d->scale, s->scale);
            d->flags = s->flags;
            s++;
            d++;
        }
    }
}
