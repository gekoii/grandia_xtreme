#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrAllGatherToLeader(void) {
    s32 i;
    ChrObj *pl;
    ChrObj *c;

    pl = ChrGetByNo(1);
    if (pl == NULL) {
        return;
    }
    for (i = 1; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (c != NULL) {
            copyq(&c->model->pos, &pl->model->pos);
            c->flags |= 1;
        }
    }
}
