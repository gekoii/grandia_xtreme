#include "common.h"
#include "gph.h"
#include "field/n14.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrTrailPush(ChrTrail *, void *, s32);

s32 ChrObjSetPos(ChrObj *o, void *v) {
    if (o == NULL) {
        return 0;
    }
    copyq(o->model->pos, v);
    ChrTrailPush(o->trail, v, 0);
    return 1;
}
