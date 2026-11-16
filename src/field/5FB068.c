#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "save.h"


static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrPartyMoveTo(ChrObj *src) {
    s32 i;
    ChrObj *c;

    if (src == NULL) {
        return;
    }
    for (i = 1; i < 4; i++) {
        c = ChrGetByNo(sysSaveData.party[i]);
        if (c != NULL) {
            copyq(&c->model->pos, &src->model->pos);
        }
    }
}
