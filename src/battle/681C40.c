#include "common.h"
#include "battle/n28.h"

f32 VuVecNormalizeLen(FVECTOR *, FVECTOR *);

FxHoming *FxHomingAdd(FxHomingPool *p, FVECTOR *a, FVECTOR *b, FVECTOR *v) {
    FxHoming *e = p->entries;
    s32 n;

    if (e == NULL) {
        return NULL;
    }
    for (n = p->count; n > 0; n--, e++) {
        if (e->state == 0) {
            break;
        }
    }
    if (n <= 0) {
        return NULL;
    }
    e->state = 0;
    e->unk4 = 0;
    e->unk8 = 0;
    e->unk10 = 0;
    e->target = a;
    e->pos = b;
    e->speed = VuVecNormalizeLen(&e->dir, v);
    e->unk14 = 0;
    return e;
}
