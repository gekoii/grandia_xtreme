#include "common.h"
#include "battle/n28.h"

void FxHomingUpdate(FxHomingPool *, FxHoming *);

void FxHomingPoolUpdate(FxHomingPool *p) {
    FxHoming *e = p->entries;
    s32 n;

    if (e != NULL) {
        for (n = p->count; n > 0; n--, e++) {
            if (e->state != 0) {
                FxHomingUpdate(p, e);
            }
        }
    }
}
